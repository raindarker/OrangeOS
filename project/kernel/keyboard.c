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

    set_irq_handler(KEYBOARD_IRQ, keyboard_handler);
    enable_irq(KEYBOARD_IRQ);
}

void keyboard_read(void) {
    u8 scan_code;
    char output[2];
    int make; /* 1: make; 0: break */

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
            /* 暂时不做任何操作 */
        } else {	/* 下面处理可打印字符 */
            /* 首先判断Make Code 还是 Break Code */
            make = (scan_code & FLAG_BREAK ? 0 : 1);
            /* 如果是Make Code 就打印，是 Break Code 则不做处理 */
            if(make) {
                output[0] = keymap[(scan_code & 0x7F) * MAP_COLS];
                disp_str(output);
            }
        }
    }
}
