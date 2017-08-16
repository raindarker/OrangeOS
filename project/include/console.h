/*
 * File Name: console.h
 * Author: Sphantix
 * Mail: hangxu@antiy.cn
 * Created Time: Mon 14 Aug 2017 07:54:23 PM CST
 */

#ifndef __ORANGE_CONSOLE_H__
#define __ORANGE_CONSOLE_H__

/* CONSOLE */
typedef struct console
{
	unsigned int current_start_addr;    /* 当前显示到了什么位置 */
	unsigned int original_addr;         /* 当前控制台对应显存位置 */
	unsigned int v_mem_limit;           /* 当前控制台占的显存大小 */
	unsigned int cursor;                /* 当前光标位置 */
} console_t;


#define SCR_UP          1   /* scroll forward */
#define SCR_DN         -1   /* scroll backward */

#define SCREEN_SIZE    (80 * 25)
#define SCREEN_WIDTH   80

#define DEFAULT_CHAR_COLOR 0x07         /* 0000 0111 黑底白字 */

#endif /* __ORANGE_CONSOLE_H__ */
