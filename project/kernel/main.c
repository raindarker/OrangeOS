/*
 * File Name: main.c
 * Author: Sphantix
 * Mail: hangxu@antiy.cn
 * Created Time: Wed 26 Jul 2017 07:41:04 PM CST
 */

#include "proto.h"
#include "proc.h"
#include "protect.h"
#include "const.h"
#include "string.h"

extern descriptor_t  gdt[GDT_SIZE];
extern process_t     process_table[NR_TASKS];
extern process_t*    process_ready;
extern char          task_stack[STACK_SIZE_TOTAL];
extern int           g_re_enter;
extern int           g_disp_pos;
extern task_t        task_table[NR_TASKS];

int kernel_main(void) {
    disp_str("-----\"kernel_main\" begins-----\n");
    task_t* task = task_table;
    process_t* process = process_table;
    char* task_stack_top = task_stack + STACK_SIZE_TOTAL;
    u16 selector_ldt = SELECTOR_LDT_FIRST;
    int i;

    for (i = 0; i < NR_TASKS; i++) {
        strcpy(process->p_name, task->name);	// name of the process
        process->pid = i;			// pid
        process->ldt_sel = selector_ldt;

        memcpy(&process->ldts[0], &gdt[SELECTOR_KERNEL_CS >> 3], sizeof(descriptor_t));
        process->ldts[0].attr1 = DA_C | PRIVILEGE_TASK << 5;
        memcpy(&process->ldts[1], &gdt[SELECTOR_KERNEL_DS >> 3], sizeof(descriptor_t));
        process->ldts[1].attr1 = DA_DRW | PRIVILEGE_TASK << 5;
        process->regs.cs = ((8 * 0) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK;
        process->regs.ds = ((8 * 1) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK;
        process->regs.es = ((8 * 1) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK;
        process->regs.fs = ((8 * 1) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK;
        process->regs.ss = ((8 * 1) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK;
        process->regs.gs = (SELECTOR_KERNEL_GS & SA_RPL_MASK) | RPL_TASK;

        process->regs.eip = (u32)task->initial_eip;
        process->regs.esp = (u32)task_stack_top;
        process->regs.eflags = 0x1202; /* IF=1, IOPL=1 */

        task_stack_top -= task->stack_size;
        process++;
        task++;
        selector_ldt += 1 << 3;
    }
    //set processes priority
    process_table[0].ticks = process_table[0].priority = 15;
    process_table[1].ticks = process_table[1].priority =  5;
    process_table[2].ticks = process_table[2].priority =  3;

    g_re_enter = 0;

    process_ready = process_table;

    init_clock();

    init_keyboard();

    restart();
    while (1) {
    }
}

void testA() {
    while (1) {
        /* disp_color_str("A.", BRIGHT | MAKE_COLOR(BLACK, RED)); */
        milli_delay(10);
    }
}

void testB() {
    int i = 0x1000;
    while (1) {
        /* disp_color_str("B.", BRIGHT | MAKE_COLOR(BLACK, GREEN)); */
        milli_delay(10);
    }
}

void testC() {
    int i = 0x2000;
    while (1) {
        /* disp_color_str("C.", BRIGHT | MAKE_COLOR(BLACK, BLUE)); */
        milli_delay(10);
    }
}
