#include "types.h"
#include "const.h"
#include "protect.h"
#include "proto.h"
#include "proc.h"
#include "tty.h"
#include "console.h"

int           g_disp_pos;
u8            gdt_ptr[6];	/* 0~15:Limit  16~47:Base */
descriptor_t  gdt[GDT_SIZE];
u8            idt_ptr[6];	/* 0~15:Limit  16~47:Base */
gate_t        idt[IDT_SIZE];

int           g_re_enter;
int           g_ticks;
int           g_current_console;

tss_t         tss;
process_t*    process_ready;

process_t     process_table[NR_TASKS];
char          task_stack[STACK_SIZE_TOTAL];
task_t        task_table[NR_TASKS] = {
    {task_tty, STACK_SIZE_TTY, "tty"},
    {testA, STACK_SIZE_TESTA, "testA"},
    {testB, STACK_SIZE_TESTB, "testB"},
    {testC, STACK_SIZE_TESTC, "testC"}
};

tty_t         g_tty_table[NR_CONSOLES];
console_t     g_console_table[NR_CONSOLES];

irq_handler   g_irq_table[NR_IRQ];

system_call   g_syscall_table[NR_SYS_CALL] = {
    sys_get_ticks
};
