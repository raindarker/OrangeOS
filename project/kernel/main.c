/*
 * File Name: main.c
 * Author: Sphantix
 * Mail: hangxu@antiy.cn
 * Created Time: Wed 26 Jul 2017 07:41:04 PM CST
 */

#include "proc.h"
#include "protect.h"
#include "const.h"
#include "string.h"
#include "tty.h"
#include "proto.h"
#include "clock.h"

extern descriptor_t  gdt[GDT_SIZE];
extern process_t     process_table[NR_TASKS];
extern process_t*    process_ready;
extern char          task_stack[STACK_SIZE_TOTAL];
extern int           g_re_enter;
extern int           g_disp_pos;
extern task_t        task_table[NR_TASKS];
extern task_t        user_proc_table[NR_PROCS];

int kernel_main(void) {
    disp_str("-----\"kernel_main\" begins-----\n");
    task_t* task = task_table;
    process_t* process = process_table;
    char* task_stack_top = task_stack + STACK_SIZE_TOTAL;
    u16 selector_ldt = SELECTOR_LDT_FIRST;
    int i;
    int prio;
    u8  privilege;
    u8  rpl;
    int eflags;

    for (i = 0; i < NR_TASKS + NR_PROCS; i++) {
        if (i < NR_TASKS) {  /* 任务 */
            task        = task_table + i;
            privilege   = PRIVILEGE_TASK;
            rpl         = RPL_TASK;
            eflags      = 0x1202; /* IF=1, IOPL=1, bit 2 is always 1 */
            prio        = 15;
        } else {            /* 用户进程 */
            task        = user_proc_table + (i - NR_TASKS);
            privilege   = PRIVILEGE_USER;
            rpl         = RPL_USER;
            eflags      = 0x202; /* IF=1, bit 2 is always 1 */
            prio        = 5;
        }

        strcpy(process->name, task->name);	// name of the process
        process->pid = i;			// pid
        process->ldt_sel = selector_ldt;

        memcpy(&process->ldts[0], &gdt[SELECTOR_KERNEL_CS >> 3], sizeof(descriptor_t));
        process->ldts[0].attr1 = DA_C | privilege << 5;
        memcpy(&process->ldts[1], &gdt[SELECTOR_KERNEL_DS >> 3], sizeof(descriptor_t));
        process->ldts[1].attr1 = DA_DRW | privilege << 5;
        process->regs.cs = ((8 * 0) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
        process->regs.ds = ((8 * 1) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
        process->regs.es = ((8 * 1) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
        process->regs.fs = ((8 * 1) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
        process->regs.ss = ((8 * 1) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
        process->regs.gs = (SELECTOR_KERNEL_GS & SA_RPL_MASK) | rpl;

        process->regs.eip = (u32)task->initial_eip;
        process->regs.esp = (u32)task_stack_top;
        process->regs.eflags = eflags;

        process->tty = 0;

		process->flags = 0;
		process->msg = 0;
		process->recvfrom = NO_TASK;
		process->sendto = NO_TASK;
		process->has_interrupt_msg = 0;
		process->sending_queue = 0;
		process->next_sending = 0;

		process->ticks = process->priority = prio;

        task_stack_top -= task->stack_size;
        process++;
        task++;
        selector_ldt += 1 << 3;
    }

    process_table[NR_TASKS + 1].tty   = 0;
    process_table[NR_TASKS + 2].tty   = 1;
    process_table[NR_TASKS + 3].tty   = 1;

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
        /* printf("<Ticks:%x>", get_ticks()); */
        milli_delay(10);
    }
}

void testB() {
    int i = 0x1000;
    while (1) {
        /* disp_color_str("B.", BRIGHT | MAKE_COLOR(BLACK, GREEN)); */
        /* printf("B"); */
        milli_delay(10);
    }
}

void testC() {
    int i = 0x2000;
    while (1) {
        /* disp_color_str("C.", BRIGHT | MAKE_COLOR(BLACK, BLUE)); */
        /* printf("C"); */
        milli_delay(10);
    }
}

void panic(const char* fmt, ...) {
	int i;
	char buf[256] = {'\0'};

	va_list arg = (va_list)((char*)&fmt + 4);
	i = vsprintf(buf, fmt, arg);

	printl("%c !!panic!! %s", MAG_CH_PANIC, buf);

	/* should never arrive here */
	__asm__ __volatile__("ud2");
}
