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

typedef char*               va_list;

typedef	void (*int_handler)(void);
typedef void (*task_func)(void);
typedef	void (*irq_handler)(int irq);

typedef void* system_call;

#endif /* __ORANGE_TYPES_H__ */
