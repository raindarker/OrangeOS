/*
 * File Name: printf.c
 * Author: Sphantix
 * Mail: hangxu@antiy.cn
 * Created Time: Thu 17 Aug 2017 07:12:02 PM CST
 */

#include "types.h"


int printf(const char *fmt, ...) {
    int i;
    char buf[256];

    va_list arg = (va_list)((char*)(&fmt) + 4);
    i = vsprintf(buf, fmt, arg);
    buf[i] = '\0';
    printx(buf);

    return i;
}

int sprintf(char* buf, const char* fmt, ...) {
    va_list args = (va_list)((char *)(&fmt) + 4);
    return vsprintf(buf, fmt, args);
}
