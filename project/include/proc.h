/*
 * File Name: proc.h
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
	char p_name[32];              /* name of the process */
} process_t;

typedef struct task {
    task_func  initial_eip;
    int        stack_size;
    char       name[32];
} task_t;

/* Number of tasks & procs */
#define NR_TASKS	1
#define NR_PROCS    3

/* stacks of tasks */
#define STACK_SIZE_TTY      0x8000
#define STACK_SIZE_TESTA	0x8000
#define STACK_SIZE_TESTB    0x8000
#define STACK_SIZE_TESTC    0x8000
#define STACK_SIZE_TOTAL    (STACK_SIZE_TTY + \
                             STACK_SIZE_TESTA + \
                             STACK_SIZE_TESTB + \
                             STACK_SIZE_TESTC)

#endif /* __ORANGE_PROC_H__ */
