/*
 * File Name: clock.c
 * Author: Sphantix
 * Mail: hangxu@antiy.cn
 * Created Time: Mon 31 Jul 2017 07:27:25 PM CST
 */

#include "proto.h"
#include "proc.h"

extern process_t  process_table[NR_TASKS];
extern process_t* process_ready;
extern int        g_re_enter;
extern int        g_ticks;

void clock_handler(int irq) {
    disp_str("#");
    g_ticks++;

    if (g_re_enter != 0) {
        disp_str("!");
        return;
    }

    process_ready++;
    if (process_ready >= process_table + NR_TASKS) {
        process_ready = process_table;
    }
}


void milli_delay(int milli_sec) {
    int t = get_ticks();
    while(((get_ticks() - t) * 1000 / HZ) < milli_sec) {}
}