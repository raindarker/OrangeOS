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

/* klib.c */
void delay(int time);

/* kernel.asm */
void restart(void);

/* main.c */
void testA(void);
#endif /* __ORANGE_PROTO_H__ */
