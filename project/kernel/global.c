#include "types.h"
#include "const.h"
#include "protect.h"
#include "proto.h"
#include "proc.h"

int           disp_pos;
u8            gdt_ptr[6];	/* 0~15:Limit  16~47:Base */
descriptor_t  gdt[GDT_SIZE];
u8            idt_ptr[6];	/* 0~15:Limit  16~47:Base */
gate_t        idt[IDT_SIZE];

int           g_re_enter;

tss_t         tss;
process_t*    process_ready;

process_t     process_table[NR_TASKS];
char          task_stack[STACK_SIZE_TOTAL];
task_t        task_table[NR_TASKS] = {
    {testA, STACK_SIZE_TESTA, "testA"},
    {testB, STACK_SIZE_TESTB, "testB"},
    {testC, STACK_SIZE_TESTC, "testC"}
};

irq_handler   g_irq_table[NR_IRQ];
