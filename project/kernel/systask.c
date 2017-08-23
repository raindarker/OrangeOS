/*
 * File Name: systask.c
 * Author: Sphantix
 * Mail: hangxu@antiy.cn
 * Created Time: Tue 22 Aug 2017 09:42:38 AM CST
 */

#include "types.h"
#include "proc.h"
#include "proto.h"

extern int           g_ticks;
extern process_t     process_table[NR_TASKS + NR_PROCS];

/*****************************************************************************
task_sys
  <Ring 1> The main loop of TASK SYS.

*****************************************************************************/
void task_sys(void) {
    message_t msg;
    while (1) {
        send_recv(RECEIVE, ANY, &msg);
        int src = msg.source;

        process_t* process = &process_table[src];

        switch (msg.type) {
        case GET_TICKS: {
            msg.RETVAL = g_ticks;
            send_recv(SEND, src, &msg);
            break;
        }
        default:
            panic("Unknown msg type!!!");
            break;
        }
    }
}
