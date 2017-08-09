/*
 * File Name: tty.c
 * Author: Sphantix
 * Mail: hangxu@antiy.cn
 * Created Time: Wed 09 Aug 2017 08:11:02 PM CST
 */

#include "types.h"
#include "const.h"
#include "proto.h"


void task_tty(void) {
    while(1) {
        keyboard_read();
    }
}
