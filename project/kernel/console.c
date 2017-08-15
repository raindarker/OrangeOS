/*
 * File Name: console.c
 * Author: Sphantix
 * Mail: hangxu@antiy.cn
 * Created Time: Tue 15 Aug 2017 07:57:07 PM CST
 */

#include "types.h"
#include "const.h"
#include "console.h"
#include "proto.h"

extern int           g_disp_pos;
extern int           g_current_console;
extern tty_t         g_tty_table[NR_CONSOLES];
extern console_t     g_console_table[NR_CONSOLES];

int is_current_console(console_t* console) {
    return (console == &g_console_table[g_current_console]);
}

static void set_cursor(unsigned int position) {
	disable_interrupt();
	out_byte(CRTC_ADDR_REG, CURSOR_H);
	out_byte(CRTC_DATA_REG, (position >> 8) & 0xFF);
	out_byte(CRTC_ADDR_REG, CURSOR_L);
	out_byte(CRTC_DATA_REG, position & 0xFF);
	enable_interrupt();
}

void out_char(console_t* console, char c) {
    u8* vmem = (u8*)(V_MEM_BASE + g_disp_pos);

    *vmem++ = c;
    *vmem++ = DEFAULT_CHAR_COLOR;
    g_disp_pos += 2;

    set_cursor(g_disp_pos/2);
}
