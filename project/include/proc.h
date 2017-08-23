/*
 * File Name: process_t.h
 * Author: Sphantix
 * Mail: hangxu@antiy.cn
 * Created Time: Wed 26 Jul 2017 07:59:45 PM CST
 */

#ifndef __ORANGE_PROC_H__
#define __ORANGE_PROC_H__
#include "const.h"
#include "types.h"
#include "protect.h"

typedef struct stack_frame {
    u32	gs;         /* \                                    */
    u32	fs;         /* |                                    */
    u32	es;         /* |                                    */
    u32	ds;         /* |                                    */
    u32	edi;        /* |                                    */
    u32	esi;        /* | pushed by save()                   */
    u32	ebp;        /* |                                    */
    u32	kernel_esp;	/* <- 'popad' will ignore it            */
    u32	ebx;        /* |                                    */
    u32	edx;        /* |                                    */
    u32	ecx;        /* |                                    */
    u32	eax;        /* /                                    */
    u32	retaddr;    /* return addr for kernel.asm::save()   */
    u32	eip;        /* \                                    */
    u32	cs;         /* |                                    */
    u32	eflags;     /* | pushed by CPU during interrupt     */
    u32	esp;        /* |                                    */
    u32	ss;         /* /                                    */
} stack_frame_t;

typedef struct process {
    stack_frame_t regs;           /* process registers saved in stack frame */
    u16 ldt_sel;                  /* gdt selector giving ldt base and limit */
    descriptor_t ldts[LDT_SIZE];  /* local descriptors for code and data */
    int ticks;
    int priority;
    u32 pid;                      /* process id passed in from MM */
    char name[32];              /* name of the process */

    // process flags
    int flags;

    // message related
    message_t* msg;
    int recvfrom;
    int sendto;
    int has_interrupt_msg;

    struct process* sending_queue;
    struct process* next_sending;

    int tty;
} process_t;

typedef struct task {
    task_func  initial_eip;
    int        stack_size;
    char       name[32];
} task_t;

#define proc2pid(x) (x - process_table)

/* Number of tasks & procs */
#define NR_TASKS	2
#define NR_PROCS    3

#define FIRST_PROCESS       process_table[0]
#define LAST_PROCESS        process_table[NR_TASKS + NR_PROCS - 1]

/* stacks of tasks */
#define STACK_SIZE_TTY      0x8000
#define STACK_SIZE_SYS      0x8000
#define STACK_SIZE_TESTA	0x8000
#define STACK_SIZE_TESTB    0x8000
#define STACK_SIZE_TESTC    0x8000
#define STACK_SIZE_TOTAL    (STACK_SIZE_TTY +   \
                             STACK_SIZE_SYS +   \
                             STACK_SIZE_TESTA + \
                             STACK_SIZE_TESTB + \
                             STACK_SIZE_TESTC)

/* tasks */
/* 注意 TASK_XXX 的定义要与 global.c 中对应 */
#define INVALID_DRIVER	-20
#define INTERRUPT       -10
#define TASK_TTY        0
#define TASK_SYS        1
/* #define TASK_WINCH 2 */
/* #define TASK_FS 3 */
/* #define TASK_MM 4 */
#define ANY            (NR_TASKS + NR_PROCS + 10)
#define NO_TASK        (NR_TASKS + NR_PROCS + 20)

/* ipc */
#define SEND           1
#define RECEIVE        2
#define BOTH           3 /* BOTH = (SEND | RECEIVE) */

#define SENDING   0x02	/* set when proc trying to send */
#define RECEIVING 0x04	/* set when proc trying to recv */

/* sys_call */
int	sys_sendrecv(int function, int src_dest, message_t* msg, process_t* p);

void  schedule();
void* va2la(int pid, void* va);
int   ldt_seg_linear(process_t* p, int idx);
void  reset_msg(message_t* msg);
void  dump_msg(const char * title, message_t* msg);
void  dump_process(process_t * p);
int	  send_recv(int function, int src_dest, message_t* msg);

#endif /* __ORANGE_PROC_H__ */
