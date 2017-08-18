/*
 * File Name: vsprintf.c
 * Author: Sphantix
 * Mail: hangxu@antiy.cn
 * Created Time: Thu 17 Aug 2017 07:17:25 PM CST
 */

#include "types.h"
#include "string.h"

int vsprintf(char* buf, const char* fmt, va_list args) {
    char* p;
    char  tmp[256];
    va_list next_arg = args;

    for (p = buf; *fmt; fmt++) {
        if (*fmt != '%') {
            *p++ = *fmt;
            continue;
        }

        fmt++;

        switch (*fmt) {
        case 'x':
            itoa(tmp, *((int*)next_arg));
            strcpy(p, tmp);
            next_arg += 4;
            p += strlen(tmp);
            break;
        case 's':
            break;
        default:
            break;
        }
    }

    return (p - buf);
}
