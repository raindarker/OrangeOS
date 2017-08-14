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


void task_tty(void) {
    while(1) {
        keyboard_read();
    }
}

void in_process(u32 key) {
    char output[2] = {'\0', '\0'};

    if (!(key & FLAG_EXT)) {
        output[0] = key & 0xFF;
        disp_str(output);
    }
}
