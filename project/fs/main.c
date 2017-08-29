/*
 * File Name: main.c
 * Author: Sphantix
 * Mail: hangxu@antiy.cn
 * Created Time: Tue 29 Aug 2017 06:47:33 PM CST
 */

#include "proto.h"
#include "hd.h"
#include "types.h"
#include "proc.h"

void task_fs(void) {
    printl("Task FS begins.\n");

    message_t msg;
    msg.type = DEV_OPEN;
    send_recv(BOTH, TASK_HD, &msg);

    spin("FS");
}
