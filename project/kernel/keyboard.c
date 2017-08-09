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

    if (kb_input_buffer.count > 0) {
        disable_interrupt();
        scan_code = *(kb_input_buffer.tail);
        kb_input_buffer.tail++;
        if (kb_input_buffer.tail == kb_input_buffer.buf + KB_IN_BYTES) {
            kb_input_buffer.tail = kb_input_buffer.buf;
        }
        kb_input_buffer.count--;
        enable_interrupt();

        disp_int(scan_code);
    }
}
