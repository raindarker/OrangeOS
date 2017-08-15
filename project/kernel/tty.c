/*
 * File Name: tty.c
 * Author: Sphantix
 * Mail: hangxu@antiy.cn
 * Created Time: Wed 09 Aug 2017 08:11:02 PM CST
 */

#include "types.h"
#include "const.h"
#include "proto.h"
#include "keyboard.h"

extern int           g_disp_pos;
extern int           g_current_console;
extern tty_t         g_tty_table[NR_CONSOLES];
extern console_t     g_console_table[NR_CONSOLES];

#define TTY_FIRST    (g_tty_table)
#define TTY_END      (g_tty_table + NR_CONSOLES)

static void init_tty(tty_t* tty) {
    tty->count = 0;
    tty->head = tty->tail = tty->buf;

    int nr_tty = tty - g_tty_table;
    tty->console = g_console_table + nr_tty;
}

static void tty_do_read(tty_t* tty) {
    if (is_current_console(tty->console)) {
        keyboard_read(tty);
    }
}

static void tty_do_write(tty_t* tty) {
    if (tty->count) {
        char c = *(tty->tail);
        tty->tail++;
        if (tty->tail == tty->buf + TTY_IN_BYTES) {
            tty->tail = tty->buf;
        }
        tty->count--;

        out_char(tty->console, c);
    }
}

void task_tty(void) {
    tty_t* tty;

    init_keyboard();

    for (tty = TTY_FIRST; tty < TTY_END; tty++) {
        init_tty(tty);
    }
    g_current_console = 0;

    while(1) {
        for (tty = TTY_FIRST; tty < TTY_END; tty++) {
            tty_do_read(tty);
            tty_do_write(tty);
        }

    }
}

void in_process(tty_t* tty, u32 key) {
    char output[2] = {'\0', '\0'};

    if (!(key & FLAG_EXT)) {
        if (tty->count < TTY_IN_BYTES) {
            *(tty->head) = key;
            tty->head++;
            if (tty->head == tty->buf + TTY_IN_BYTES) {
                tty->head = tty->buf;
            }
            tty->count++;
        }
    } else {
        int raw_code = key & MASK_RAW;
        switch(raw_code) {
        case UP:
            if ((key & FLAG_SHIFT_L) || (key & FLAG_SHIFT_R)) {
                disable_interrupt();
                out_byte(CRTC_ADDR_REG, START_ADDR_H);
                out_byte(CRTC_DATA_REG, ((80*15) >> 8) & 0xFF);
                out_byte(CRTC_ADDR_REG, START_ADDR_L);
                out_byte(CRTC_DATA_REG, (80*15) & 0xFF);
                enable_interrupt();
            }
            break;
        case DOWN:
            if ((key & FLAG_SHIFT_L) || (key & FLAG_SHIFT_R)) {
                /* Shift+Down, do nothing */
            }
            break;
        default:
            break;
        }
    }
}
