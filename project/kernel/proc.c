/*
 * File Name: process_t.c
 * Author: Sphantix
 * Mail: hangxu@antiy.cn
 * Created Time: Thu 03 Aug 2017 09:55:22 AM CST
 */

#include "types.h"
#include "const.h"
#include "proto.h"
#include "proc.h"
#include "assert.h"
#include "string.h"

extern int           g_ticks;
extern int           g_re_enter;
extern process_t     process_table[NR_TASKS];
extern process_t*    process_ready;

void schedule(void) {
    process_t* p;
    int  greatest_ticks = 0;

    while (!greatest_ticks) {
        for (p = &FIRST_PROCESS; p < &LAST_PROCESS; p++) {
            if (p->flags == 0) {
                if (p->ticks > greatest_ticks) {
                    greatest_ticks = p->ticks;
                    process_ready = p;
                }
            }
        }

        if (!greatest_ticks) {
            for (p = &FIRST_PROCESS; p < &LAST_PROCESS; p++) {
                if (p->flags == 0) {
                    p->ticks = p->priority;
                }
            }
        }
    }
}

int ldt_seg_linear(process_t* p, int idx) {
    descriptor_t* d = &p->ldts[idx];

    return d->base_high << 24 | d->base_mid << 16 | d->base_low;
}

void* va2la(int pid, void* va) {
    process_t* p = &process_table[pid];

    u32 seg_base = ldt_seg_linear(p, INDEX_LDT_RW);
    u32 la = seg_base + (u32)va;

    if (pid < NR_TASKS + NR_PROCS) {
        assert(la == (u32)va);
    }

    return (void*)la;
}

void reset_msg(message_t* msg) {
    memset(msg, 0, sizeof(message_t));
}

int send_recv(int function, int src_dest, message_t* msg) {
    int ret = 0;

    if (function == RECEIVE)
        memset(msg, 0, sizeof(message_t));

    switch (function) {
    case BOTH:
        ret = sendrecv(SEND, src_dest, msg);
        if (ret == 0)
            ret = sendrecv(RECEIVE, src_dest, msg);
        break;
    case SEND:
    case RECEIVE:
        ret = sendrecv(function, src_dest, msg);
        break;
    default:
        assert((function == BOTH) ||
               (function == SEND) || (function == RECEIVE));
        break;
    }

    return ret;
}

static void block(process_t* p) {
    assert(p->flags);
    schedule();
}

static void unblock(process_t* p) {
    assert(p->flags == 0);
}

static int deadlock(int src, int dest) {
    process_t* p = process_table + dest;
    while (1) {
        if (p->flags & SENDING) {
            if (p->sendto == src) {
                /* print the chain */
                p = process_table + dest;
                printl("=_=%s", p->name);
                do {
                    assert(p->msg);
                    p = process_table + p->sendto;
                    printl("->%s", p->name);
                } while (p != process_table + src);
                printl("=_=");

                return 1;
            }
            p = process_table + p->sendto;
        }
        else {
            break;
        }
    }
    return 0;
}

static int msg_send(process_t* current, int dst, message_t* msg) {
    process_t* sender = current;
    process_t* dest = process_table + dst; /* process_t dest */

    assert(proc2pid(sender) != dst);

    /* check for deadlock here */
    if (deadlock(proc2pid(sender), dst)) {
        panic(">>DEADLOCK<< %s->%s", sender->name, dest->name);
    }

    if ((dest->flags & RECEIVING) && /* dest is waiting for the msg */
        (dest->recvfrom == proc2pid(sender) ||
         dest->recvfrom == ANY)) {
        assert(dest->msg);
        assert(msg);

        phys_copy(va2la(dst, dest->msg),
                  va2la(proc2pid(sender), msg),
                  sizeof(message_t));
        dest->msg = 0;
        dest->flags &= ~RECEIVING; /* dest has received the msg */
        dest->recvfrom = NO_TASK;
        unblock(dest);

        assert(dest->flags == 0);
        assert(dest->msg == 0);
        assert(dest->recvfrom == NO_TASK);
        assert(dest->sendto == NO_TASK);
        assert(sender->flags == 0);
        assert(sender->msg == 0);
        assert(sender->recvfrom == NO_TASK);
        assert(sender->sendto == NO_TASK);
    } else { /* dest is not waiting for the msg */
        sender->flags |= SENDING;
        assert(sender->flags == SENDING);
        sender->sendto = dst;
        sender->msg = msg;

        /* append to the sending_queue queue */
        process_t * p;
        if (dest->sending_queue) {
            p = dest->sending_queue;
            while (p->next_sending)
                p = p->next_sending;
            p->next_sending = sender;
        } else {
            dest->sending_queue = sender;
        }
        sender->next_sending = 0;

        block(sender);

        assert(sender->flags == SENDING);
        assert(sender->msg != 0);
        assert(sender->recvfrom == NO_TASK);
        assert(sender->sendto == dst);
    }

    return 0;
}

static int msg_receive(process_t* current, int src, message_t* msg) {
    process_t* who_wanna_recv = current; /**
                                          * This name is a little bit
                                          * wierd, but it makes me
                                          * think clearly, so I keep
                                          * it.
                                          */
    process_t* from = 0; /* from which the message will be fetched */
    process_t* prev = 0;
    int copy_ok = 0;

    assert(proc2pid(who_wanna_recv) != src);

    if ((who_wanna_recv->has_interrupt_msg) &&
        ((src == ANY) || (src == INTERRUPT))) {
        /* There is an interrupt needs who_wanna_recv's handling and
         * who_wanna_recv is ready to handle it.
         */
        message_t m;
        reset_msg(&m);
        m.source = INTERRUPT;
        m.type = HARD_INT;
        assert(msg);
        phys_copy(va2la(proc2pid(who_wanna_recv), msg), &m, sizeof(message_t));

        who_wanna_recv->has_interrupt_msg = 0;

        assert(who_wanna_recv->flags == 0);
        assert(who_wanna_recv->msg == 0);
        assert(who_wanna_recv->sendto == NO_TASK);
        assert(who_wanna_recv->has_interrupt_msg == 0);

        return 0;
    }

    /* Arrives here if no interrupt for who_wanna_recv. */
    if (src == ANY) {
        /* who_wanna_recv is ready to receive messages from
         * ANY process_t, we'll check the sending_queue queue and pick the
         * first process_t in it.
         */
        if (who_wanna_recv->sending_queue) {
            from = who_wanna_recv->sending_queue;
            copy_ok = 1;

            assert(who_wanna_recv->flags == 0);
            assert(who_wanna_recv->msg == 0);
            assert(who_wanna_recv->recvfrom == NO_TASK);
            assert(who_wanna_recv->sendto == NO_TASK);
            assert(who_wanna_recv->sending_queue != 0);
            assert(from->flags == SENDING);
            assert(from->msg != 0);
            assert(from->recvfrom == NO_TASK);
            assert(from->sendto == proc2pid(who_wanna_recv));
        }
    } else {
        /* who_wanna_recv wants to receive a message from
         * a certain process_t: src.
         */
        from = &process_table[src];

        if ((from->flags & SENDING) &&
            (from->sendto == proc2pid(who_wanna_recv))) {
            /* Perfect, src is sending_queue a message to
             * who_wanna_recv.
             */
            copy_ok = 1;

            process_t* p = who_wanna_recv->sending_queue;
            assert(p); /* from must have been appended to the
                        * queue, so the queue must not be NULL
                        */
            while (p) {
                assert(from->flags & SENDING);
                if (proc2pid(p) == src) { /* if p is the one */
                    from = p;
                    break;
                }
                prev = p;
                p = p->next_sending;
            }

            assert(who_wanna_recv->flags == 0);
            assert(who_wanna_recv->msg == 0);
            assert(who_wanna_recv->recvfrom == NO_TASK);
            assert(who_wanna_recv->sendto == NO_TASK);
            assert(who_wanna_recv->sending_queue != 0);
            assert(from->flags == SENDING);
            assert(from->msg != 0);
            assert(from->recvfrom == NO_TASK);
            assert(from->sendto == proc2pid(who_wanna_recv));
        }
    }

    if (copy_ok) {
        /* It's determined from which process_t the message will
         * be copied. Note that this process_t must have been
         * waiting for this moment in the queue, so we should
         * remove it from the queue.
         */
        if (from == who_wanna_recv->sending_queue) { /* the 1st one */
            assert(prev == 0);
            who_wanna_recv->sending_queue = from->next_sending;
            from->next_sending = 0;
        } else {
            assert(prev);
            prev->next_sending = from->next_sending;
            from->next_sending = 0;
        }

        assert(msg);
        assert(from->msg);
        /* copy the message */
        phys_copy(va2la(proc2pid(who_wanna_recv), msg),
                  va2la(proc2pid(from), from->msg),
                  sizeof(message_t));

        from->msg = 0;
        from->sendto = NO_TASK;
        from->flags &= ~SENDING;
        unblock(from);
    } else {  /* nobody's sending_queue any msg
                 Set flags so that who_wanna_recv will not
                 * be scheduled until it is unblocked.
                 */
        who_wanna_recv->flags |= RECEIVING;
        who_wanna_recv->msg = msg;

        if (src == ANY)
            who_wanna_recv->recvfrom = ANY;
        else
            who_wanna_recv->recvfrom = proc2pid(from);

        block(who_wanna_recv);

        assert(who_wanna_recv->flags == RECEIVING);
        assert(who_wanna_recv->msg != 0);
        assert(who_wanna_recv->recvfrom != NO_TASK);
        assert(who_wanna_recv->sendto == NO_TASK);
        assert(who_wanna_recv->has_interrupt_msg == 0);
    }

    return 0;
}

int sys_sendrecv(int function, int src_dest, message_t* msg, process_t* p) {
    assert(g_re_enter == 0);	/* make sure we are not in ring0 */
    assert((src_dest >= 0 && src_dest < NR_TASKS + NR_PROCS) ||
           src_dest == ANY ||
           src_dest == INTERRUPT);

    int ret = 0;
    int caller = proc2pid(p);
    message_t* mla = (message_t*)va2la(caller, msg);
    mla->source = caller;

    assert(mla->source != src_dest);

    /**
     * Actually we have the third message type: BOTH. However, it is not
     * allowed to be passed to the kernel directly. Kernel doesn't know
     * it at all. It is transformed into a SEND followed by a RECEIVE
     * by `send_recv()'.
     */
    if (function == SEND) {
        ret = msg_send(p, src_dest, msg);
        if (ret != 0)
            return ret;
    }
    else if (function == RECEIVE) {
        ret = msg_receive(p, src_dest, msg);
        if (ret != 0)
            return ret;
    }
    else {
        panic("{sys_sendrecv} invalid function: "
              "%d (SEND:%d, RECEIVE:%d).", function, SEND, RECEIVE);
    }

    return 0;
}

void inform_interrupt(int task) {
    process_t* p = process_table + task;

    if ((p->flags & RECEIVING) && /* dest is waiting for the msg */
        ((p->recvfrom == INTERRUPT) || (p->recvfrom == ANY))) {
        p->msg->source = INTERRUPT;
        p->msg->type = HARD_INT;
        p->msg = 0;
        p->has_interrupt_msg = 0;
        p->flags &= ~RECEIVING; /* dest has received the msg */
        p->recvfrom = NO_TASK;

        assert(p->flags == 0);
        unblock(p);

        assert(p->flags == 0);
        assert(p->msg == 0);
        assert(p->recvfrom == NO_TASK);
        assert(p->sendto == NO_TASK);
    } else {
        p->has_interrupt_msg = 1;
    }
}
