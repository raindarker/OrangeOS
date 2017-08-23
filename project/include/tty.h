/*
 * File Name: tty.h
 * Author: Sphantix
 * Mail: hangxu@antiy.cn
 * Created Time: Tue 15 Aug 2017 06:57:00 PM CST
 */

#ifndef __ORANGE_TTY_H__
#define __ORANGE_TTY_H__

#include "types.h"

typedef struct process process_t;
typedef struct console console_t;

#define TTY_IN_BYTES    256 /* tty input queue size */

/* TTY */
typedef struct tty {
	u32  buf[TTY_IN_BYTES];   /* TTY 输入缓冲区 */
	u32* head;                /* 指向缓冲区中下一个空闲位置 */
	u32* tail;                /* 指向键盘任务应处理的键值 */
	int  count;               /* 缓冲区中已经填充了多少 */

	console_t* console;
} tty_t;

/* magic chars used by `printx' */
#define MAG_CH_PANIC	'\002'
#define MAG_CH_ASSERT	'\003'

void task_tty(void);
void in_process(tty_t* tty, u32 key);
int	sys_printx(int _unused1, int _unused2, char* s, process_t * p);

#endif /* __ORANGE_TTY_H__ */
