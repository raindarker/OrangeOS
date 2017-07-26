/*
 * File Name: main.c
 * Author: Sphantix
 * Mail: hangxu@antiy.cn
 * Created Time: Wed 26 Jul 2017 07:41:04 PM CST
 */

#include "proto.h"

int kernel_main(void) {
    disp_str("-----\"kernel_main\" begins-----\n");
    while (1) {

    }
}

void testA() {
    int i = 0;
    while (1) {
        disp_str("A");
        disp_int(i++);
        disp_str(".");
        delay(1);
    }
}
