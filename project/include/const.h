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

/* Hardware interrupts */
#define	NR_IRQ          16	/* Number of IRQs */
#define	CLOCK_IRQ	    0
#define	KEYBOARD_IRQ	1
#define	CASCADE_IRQ	    2   /* cascade enable for 2nd AT controller */
#define	ETHER_IRQ	    3   /* default ethernet interrupt vector */
#define	SECONDARY_IRQ	3   /* RS232 interrupt vector for port 2 */
#define	RS232_IRQ	    4   /* RS232 interrupt vector for port 1 */
#define	XT_WINI_IRQ	    5   /* xt winchester */
#define	FLOPPY_IRQ	    6   /* floppy disk */
#define	PRINTER_IRQ	    7
#define	AT_WINI_IRQ	    14	/* at winchester */

/* system call */
#define NR_SYS_CALL     1

#endif /* __ORANGE_CONST_H__ */
