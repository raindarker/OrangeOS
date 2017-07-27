/*
 * File Name: const.h
 * Author: Sphantix
 * Mail: hangxu@antiy.cn
 * Created Time: Mon 24 Jul 2017 10:35:48 AM CST
 */

#ifndef __ORANGE_CONST_H__
#define __ORANGE_CONST_H__

/* GDT 和 IDT 中描述符的个数 */
#define	GDT_SIZE	128
#define	IDT_SIZE	256
/* 每个任务有一个单独的 LDT, 每个 LDT 中的描述符个数: */
#define LDT_SIZE    2

/* 权限 */
#define	PRIVILEGE_KRNL	0
#define	PRIVILEGE_TASK	1
#define	PRIVILEGE_USER	3

/* 8259A interrupt controller ports. */
#define INT_M_CTL     0x20 /* I/O port for interrupt controller       <Master> */
#define INT_M_CTLMASK 0x21 /* setting bits in this port disables ints <Master> */
#define INT_S_CTL     0xA0 /* I/O port for second interrupt controller<Slave>  */
#define INT_S_CTLMASK 0xA1 /* setting bits in this port disables ints <Slave>  */


#endif /* __ORANGE_CONST_H__ */
