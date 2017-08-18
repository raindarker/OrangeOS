/*
 * File Name: proto.h
 * Author: Sphantix
 * Mail: hangxu@antiy.cn
 * Created Time: Wed 26 Jul 2017 07:41:04 PM CST
 */

#ifndef __ORANGE_PROTO_H__
#define __ORANGE_PROTO_H__
#include "types.h"
#include "tty.h"
#include "proc.h"

/* kliba.asm */
void out_byte(u16 port, u8 value);
u8 in_byte(u16 port);
void disp_str(char * info);
void disp_color_str(char * info, int color);
void enable_irq(int irq);
void disable_irq(int irq);
void enable_interrupt(void);
void disable_interrupt(void);

/* protect.c */
void init_prot();
void init_8259A();
u32	seg2phys(u16 seg);

/* klib.c */
void delay(int time);

/* kernel.asm */
void restart(void);

/* main.c */
void testA(void);
void testB(void);
void testC(void);

/* i8259.c */
void set_irq_handler(int irq, irq_handler handler);
void spurious_irq(int irq);

/* clock.c */
void clock_handler(int irq);
void milli_delay(int milli_sec);
void init_clock(void);

/* keyboard.c */
void init_keyboard(void);
void keyboard_read(tty_t* tty);

/* tty.c */
void task_tty(void);
void in_process(tty_t* tty, u32 key);

/* proc.c */
int sys_get_ticks(void);        /* sys_call */
void schedule(void);

/* console.c */
int is_current_console(console_t* console);
void out_char(console_t* console, char c);
void init_screen(tty_t* tty);
void select_console(int nr_console);
void scroll_screen(console_t* console, int direction);

/* printf.c */
int printf(const char* fmt, ...);

/* vsprintf.c */
int vsprintf(char* buf, const char* fmt, va_list args);

/* syscall */

/* system */
/* proc.c */
int  sys_get_ticks(void);
int  sys_write(char* buf, int len, process_t* process);
/* syscall.asm */
void sys_call(void);             /* int_handler */

/* userland */
int  get_ticks(void);
void write(char* buf, int len);



#endif /* __ORANGE_PROTO_H__ */
