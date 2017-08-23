/*
 * File Name: misc.c
 * Author: Sphantix
 * Mail: hangxu@antiy.cn
 * Created Time: Tue 22 Aug 2017 10:09:29 AM CST
 */
#include "proto.h"
#include "tty.h"

void spin(char* function_name) {
	printl("\nspinning in %s ...\n", function_name);
	while (1) {}
}

void assertion_failure(char* exp, char* file, char* base_file, int line) {
    printl("%c  assert(%s) failed: file: %s, base_file: %s, ln%d",
           MAG_CH_ASSERT,
           exp, file, base_file, line);

    /**
     * If assertion fails in a TASK, the system will halt before
     * printl() returns. If it happens in a USER PROC, printl() will
     * return like a common routine and arrive here.
     * @see sys_printx()
     *
     * We use a forever loop to prevent the proc from going on:
     */
    spin("assertion_failure()");

    /* should never arrive here */
    __asm__ __volatile__("ud2");
}
