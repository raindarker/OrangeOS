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

extern descriptor_t  gdt[GDT_SIZE];
extern process_t     process_table[NR_TASKS];
extern process_t*    process_ready;
extern char          task_stack[STACK_SIZE_TOTAL];
extern int           g_re_enter;

int kernel_main(void) {
    disp_str("-----\"kernel_main\" begins-----\n");
    process_t* process = process_table;

	process->ldt_sel	= SELECTOR_LDT_FIRST;
	memcpy(&process->ldts[0], &gdt[SELECTOR_KERNEL_CS >> 3], sizeof(descriptor_t));
	process->ldts[0].attr1 = DA_C | PRIVILEGE_TASK << 5;	// change the DPL
	memcpy(&process->ldts[1], &gdt[SELECTOR_KERNEL_DS >> 3], sizeof(descriptor_t));
	process->ldts[1].attr1 = DA_DRW | PRIVILEGE_TASK << 5;	// change the DPL

	process->regs.cs = ((8 * 0) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK;
	process->regs.ds = ((8 * 1) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK;
	process->regs.es = ((8 * 1) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK;
	process->regs.fs = ((8 * 1) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK;
	process->regs.ss = ((8 * 1) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK;
	process->regs.gs = (SELECTOR_KERNEL_GS & SA_RPL_MASK) | RPL_TASK;
	process->regs.eip = (u32)testA;
	process->regs.esp = (u32)(task_stack + STACK_SIZE_TOTAL);
	process->regs.eflags = 0x1202;	// IF=1, IOPL=1, bit 2 is always 1.

    g_re_enter = -1;

	process_ready = process_table;
	restart();
    while (1) {
    }
}

void testA() {
    int i = 0;
    while (1) {
        disp_str("A");
        disp_int(i++);
        disp_str(".");
        delay(1);
    }
}
