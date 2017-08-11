/*
 * File Name: keyboard.c
 * Author: Sphantix
 * Mail: hangxu@antiy.cn
 * Created Time: Tue 08 Aug 2017 09:40:33 AM CST
 */

#include "types.h"
#include "const.h"
#include "proto.h"
#include "keyboard.h"
#include "keymap.h"

static kb_input_t kb_input_buffer;

static int code_with_E0 = 0;
static int shift_l;            /* l shift state */
static int shift_r;            /* r shift state */
static int alt_l;              /* l alt state    */
static int alt_r;              /* r left state   */
static int ctrl_l;             /* l ctrl state   */
static int ctrl_r;             /* l ctrl state   */
static int caps_lock;          /* Caps Lock  */
static int num_lock;           /* Num Lock   */
static int scroll_lock;        /* Scroll Lock    */
static int column;

void keyboard_handler(int irq) {
    /* disp_str("*"); */
    u8 scan_code = in_byte(KB_DATA);

    if (kb_input_buffer.count < KB_IN_BYTES) {
        *(kb_input_buffer.head) = scan_code;
        kb_input_buffer.head++;
        if (kb_input_buffer.head == kb_input_buffer.buf + KB_IN_BYTES) {
            kb_input_buffer.head = kb_input_buffer.buf;
        }
        kb_input_buffer.count++;
    }
}

void init_keyboard(void) {
    kb_input_buffer.count = 0;
    kb_input_buffer.head = kb_input_buffer.tail = kb_input_buffer.buf;

    shift_l = shift_r = 0;
    alt_l = alt_r = 0;
    ctrl_l = ctrl_r = 0;

    set_irq_handler(KEYBOARD_IRQ, keyboard_handler);
    enable_irq(KEYBOARD_IRQ);
}

void keyboard_read(void) {
    u8 scan_code;
    char output[2];
    int make; /* 1: make; 0: break */
    u32 key = 0;
    u32* key_row;

    if (kb_input_buffer.count > 0) {
        disable_interrupt();
        scan_code = *(kb_input_buffer.tail);
        kb_input_buffer.tail++;
        if (kb_input_buffer.tail == kb_input_buffer.buf + KB_IN_BYTES) {
            kb_input_buffer.tail = kb_input_buffer.buf;
        }
        kb_input_buffer.count--;
        enable_interrupt();

        /* 下面开始解析扫描码 */
        if (scan_code == 0xE1) {
            /* 暂时不做任何操作 */
        } else if (scan_code == 0xE0) {
            code_with_E0 = 1;
        } else {	/* 下面处理可打印字符 */
            /* 首先判断Make Code 还是 Break Code */
            make = (scan_code & FLAG_BREAK ? 0 : 1);

            key_row = &keymap[(scan_code & 0x7F) * MAP_COLS];

            column = 0;
            if (shift_l || shift_r) {
                column = 1;
            }

            if (code_with_E0) {
                column = 2;
                code_with_E0 = 0;
            }

            key = key_row[column];

            switch(key) {
            case SHIFT_L:
                shift_l = make;
                key = 0;
                break;
            case SHIFT_R:
                shift_r = make;
                key = 0;
                break;
            case CTRL_L:
                ctrl_l = make;
                key = 0;
                break;
            case CTRL_R:
                ctrl_r = make;
                key = 0;
                break;
            case ALT_L:
                alt_l = make;
                key = 0;
                break;
            case ALT_R:
                alt_l = make;
                key = 0;
                break;
            default:
                if (!make) {	/* 如果是 Break Code */
                    key = 0;	/* 忽略之 */
                }
                break;
            }

            /* 如果是Make Code 就打印，是 Break Code 则不做处理 */
            if(key) {
                output[0] = key;
                disp_str(output);
            }
        }
    }
}
