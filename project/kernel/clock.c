/*
 * File Name: clock.c
 * Author: Sphantix
 * Mail: hangxu@antiy.cn
 * Created Time: Mon 31 Jul 2017 07:27:25 PM CST
 */

#include "proto.h"
#include "proc.h"

extern process_t process_table[NR_TASKS];
extern process_t* process_ready;

void clock_handler(int irq) {
    disp_str("#");
    process_ready++;
    if (process_ready >= process_table + NR_TASKS) {
        process_ready = process_table;
    }
}
