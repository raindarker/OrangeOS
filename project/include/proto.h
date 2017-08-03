/*
 * File Name: proto.h
 * Author: Sphantix
 * Mail: hangxu@antiy.cn
 * Created Time: Wed 26 Jul 2017 07:41:04 PM CST
 */

#ifndef __ORANGE_PROTO_H__
#define __ORANGE_PROTO_H__
#include "types.h"

/* kliba.asm */
void out_byte(u16 port, u8 value);
u8 in_byte(u16 port);
void disp_str(char * info);
void disp_color_str(char * info, int color);

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

/* proc.c */
int sys_get_ticks(void);        /* sys_call */

/* syscall.asm */
void sys_call(void);             /* int_handler */
int get_ticks(void);

#endif /* __ORANGE_PROTO_H__ */
