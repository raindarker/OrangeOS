/*
 * File Name: console.h
 * Author: Sphantix
 * Mail: hangxu@antiy.cn
 * Created Time: Mon 14 Aug 2017 07:54:23 PM CST
 */

#ifndef __ORANGE_CONSOLE_H__
#define __ORANGE_CONSOLE_H__

typedef struct tty tty_t;

/* CONSOLE */
typedef struct console {
	unsigned int current_start_addr;    /* 当前显示到了什么位置 */
	unsigned int original_addr;         /* 当前控制台对应显存位置 */
	unsigned int v_mem_limit;           /* 当前控制台占的显存大小 */
	unsigned int cursor;                /* 当前光标位置 */
} console_t;


#define SCR_UP          1   /* scroll forward */
#define SCR_DN         -1   /* scroll backward */

#define SCREEN_SIZE    (80 * 25)
#define SCREEN_WIDTH   80

/* Color */
/*
 * e.g. MAKE_COLOR(BLUE, RED)
 *      MAKE_COLOR(BLACK, RED) | BRIGHT
 *      MAKE_COLOR(BLACK, RED) | BRIGHT | FLASH
 */
#define BLACK   0x0     /* 0000 */
#define WHITE   0x7     /* 0111 */
#define RED     0x4     /* 0100 */
#define GREEN   0x2     /* 0010 */
#define BLUE    0x1     /* 0001 */
#define FLASH   0x80    /* 1000 0000 */
#define BRIGHT  0x08    /* 0000 1000 */
#define MAKE_COLOR(x,y) (x | y) /* MAKE_COLOR(Background,Foreground) */

#define DEFAULT_CHAR_COLOR      0x07         /* 0000 0111 黑底白字 */
#define GRAY_CHAR		        (MAKE_COLOR(BLACK, BLACK) | BRIGHT)
#define RED_CHAR		        (MAKE_COLOR(BLUE, RED) | BRIGHT)

int is_current_console(console_t* console);
void out_char(console_t* console, char c);
void init_screen(tty_t* tty);
void select_console(int console);
void scroll_screen(console_t* console, int direction);

#endif /* __ORANGE_CONSOLE_H__ */
