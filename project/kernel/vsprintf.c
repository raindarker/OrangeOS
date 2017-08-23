/*
 * File Name: vsprintf.c
 * Author: Sphantix
 * Mail: hangxu@antiy.cn
 * Created Time: Thu 17 Aug 2017 07:17:25 PM CST
 */

#include "types.h"
#include "const.h"
#include "string.h"

static char* i2a(int val, int base, char** ps)
{
    int m = val % base;
    int q = val / base;
    if (q) {
        i2a(q, base, ps);
    }
    *(*ps)++ = (m < 10) ? (m + '0') : (m - 10 + 'A');

    return *ps;
}

int vsprintf(char* buf, const char* fmt, va_list args) {
    char cs;
    char* p;
    char* q;
    int m;
    int k;
    int align_nr;
    char inner_buf[STR_DEFAULT_LEN] = {'\0'};
    va_list next_arg = args;

    for (p = buf; *fmt; fmt++) {
        if (*fmt != '%') {
            *p++ = *fmt;
            continue;
        } else {
            align_nr = 0;
        }

        fmt++;

        if (*fmt == '%') {
            *p++ = *fmt;
            continue;
        } else if (*fmt == '0') {
            cs = '0';
            fmt++;
        } else {
            cs = ' ';
        }

        while (((unsigned char)(*fmt) >= '0' && (unsigned char)(*fmt) <= '9')) {
            align_nr *= 10;
            align_nr += *fmt - '0';
            fmt++;
        }

        q = inner_buf;
        memset(q, 0, sizeof(inner_buf));

        switch (*fmt) {
        case 'c':
            *q++ = *((char*)next_arg);
            next_arg += 4;
            break;
        case 'x':
            m = *((int *)next_arg);
            if (m < 0) {
                m = m * (-1);
                *q++ = '-';
            }
            i2a(m, 10, &q);
            next_arg += 4;
            break;
        case 'd':
            m = *((int*)next_arg);
            if (m < 0) {
                m = m * (-1);
                *q++ = '-';
            }
            i2a(m, 10, &q);
            next_arg += 4;
            break;
        case 's':
            strcpy(q, (*((char**)next_arg)));
            q += strlen(*((char**)next_arg));
            next_arg += 4;
            break;
        default:
            break;
        }
    }

    for (k = 0; k < ((align_nr > strlen(inner_buf)) ? (align_nr - strlen(inner_buf)) : 0); k++) {
        *p++ = cs;
    }
    q = inner_buf;
    while (*q) {
        *p++ = *q++;
    }
    return (p - buf);
}
