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

tss_t         tss;
process_t*    process_ready;

process_t     process_table[NR_TASKS];
char          task_stack[STACK_SIZE_TOTAL];
