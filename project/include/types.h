/*
 * File Name: types.h
 * Author: Sphantix
 * Mail: hangxu@antiy.cn
 * Created Time: Mon 24 Jul 2017 10:39:58 AM CST
 */

#ifndef __ORANGE_TYPES_H__
#define __ORANGE_TYPES_H__

typedef	unsigned int        u32;
typedef	unsigned short      u16;
typedef	unsigned char       u8;

typedef	void (*int_handler)();
typedef void (*task_func)();
typedef	void (*irq_handler)(int irq);

#endif /* __ORANGE_TYPES_H__ */
