/*
 * File Name: console.c
 * Author: Sphantix
 * Mail: hangxu@antiy.cn
 * Created Time: Tue 15 Aug 2017 07:57:07 PM CST
 */

#include "types.h"
#include "const.h"
#include "tty.h"
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

static void set_video_start_addr(u32 addr) {
    disable_interrupt();
    out_byte(CRTC_ADDR_REG, START_ADDR_H);
    out_byte(CRTC_DATA_REG, (addr >> 8) & 0xFF);
    out_byte(CRTC_ADDR_REG, START_ADDR_L);
    out_byte(CRTC_DATA_REG, addr & 0xFF);
    enable_interrupt();
}

static void flush(console_t* console) {
    set_cursor(console->cursor);
    set_video_start_addr(console->current_start_addr);
}

void out_char(console_t* console, char c) {
    u8* vmem = (u8*)(V_MEM_BASE + console->cursor * 2);

    switch(c) {
    case '\n':
        if (console->cursor < console->original_addr +
            console->v_mem_limit - SCREEN_WIDTH) {
            console->cursor = console->original_addr + SCREEN_WIDTH *
                ((console->cursor - console->original_addr) /
                 SCREEN_WIDTH + 1);
        }
        break;
    case '\b':
        if (console->cursor > console->original_addr) {
            console->cursor--;
            *(vmem - 2) = ' ';
            *(vmem - 1) = DEFAULT_CHAR_COLOR;
        }
        break;
    default:
        if (console->cursor <
            console->original_addr + console->v_mem_limit - 1) {
            *vmem++ = c;
            *vmem++ = DEFAULT_CHAR_COLOR;
            console->cursor++;
        }
        break;
    }

    while (console->cursor >= console->current_start_addr + SCREEN_SIZE) {
        scroll_screen(console, SCR_DN);
    }

    flush(console);
}

void init_screen(tty_t* tty) {
    int nr_tty = tty - g_tty_table;
    tty->console = g_console_table + nr_tty;

    int v_mem_size = V_MEM_SIZE >> 1;
    int console_v_mem_size           = v_mem_size / NR_CONSOLES;
    tty->console->original_addr      = nr_tty * console_v_mem_size;
    tty->console->v_mem_limit        = console_v_mem_size;
    tty->console->current_start_addr = tty->console->original_addr;
    /* 默认光标位置在最开始处 */
    tty->console->cursor             = tty->console->original_addr;

    if (nr_tty == 0) {
        /* 第一个控制台沿用原来的光标位置 */
        tty->console->cursor = g_disp_pos / 2;
        g_disp_pos = 0;
    } else {
        out_char(tty->console, nr_tty + '0');
        out_char(tty->console, '#');
    }

    set_cursor(tty->console->cursor);
}

void select_console(int nr_console) {
    if ((nr_console < 0) || (nr_console >= NR_CONSOLES)) {
        return;
    }

    g_current_console = nr_console;

    flush(&g_console_table[nr_console]);
}

void scroll_screen(console_t* console, int direction) {
    if (direction == SCR_UP) {
        if (console->current_start_addr > console->original_addr) {
            console->current_start_addr -= SCREEN_WIDTH;
        }
    } else if (direction == SCR_DN) {
        if (console->current_start_addr + SCREEN_SIZE <
            console->original_addr + console->v_mem_limit) {
            console->current_start_addr += SCREEN_WIDTH;
        }
    } else{
    }

    flush(console);
}
