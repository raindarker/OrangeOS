/*
 * File Name: proc.c
 * Author: Sphantix
 * Mail: hangxu@antiy.cn
 * Created Time: Thu 03 Aug 2017 09:55:22 AM CST
 */

#include "types.h"
#include "const.h"
#include "proto.h"
#include "proc.h"

extern int g_ticks;
/*======================================================================*
  sys_get_ticks
  *======================================================================*/
int sys_get_ticks(void) {
	return g_ticks;
}
