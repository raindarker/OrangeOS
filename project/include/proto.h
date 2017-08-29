/*
 * File Name: proto.h
 * Author: Sphantix
 * Mail: hangxu@antiy.cn
 * Created Time: Wed 26 Jul 2017 07:41:04 PM CST
 */

#ifndef __ORANGE_PROTO_H__
#define __ORANGE_PROTO_H__
#include "types.h"
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
void port_read(u16 port, void* buf, int n);

/* protect.c */
void init_prot();
void init_8259A();
u32	seg2phys(u16 seg);

/* klib.c */
void  delay(int time);
void  disp_int(int input);
char* itoa(char * str, int num);

/* kernel.asm */
void restart(void);

/* main.c */
void testA(void);
void testB(void);
void testC(void);
void panic(const char* fmt, ...);

/* i8259.c */
void init_8259A();
void set_irq_handler(int irq, irq_handler handler);
void spurious_irq(int irq);

/* systask.c */
void task_sys();

/* printf.c */
int printf(const char* fmt, ...);
#define printl printf
int sprintf(char* buf, const char* fmt, ...);

/* vsprintf.c */
int vsprintf(char* buf, const char* fmt, va_list args);

/* syscall */

/* syscall.asm */
void sys_call(void);             /* int_handler */

/* userland */
int  sendrecv(int function, int src_dest, message_t* msg);
void printx(char* buf);



#endif /* __ORANGE_PROTO_H__ */
