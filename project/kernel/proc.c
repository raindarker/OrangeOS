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

extern int           g_ticks;
extern process_t     process_table[NR_TASKS];
extern process_t*    process_ready;

/*======================================================================*
  schedule
  *======================================================================*/
void schedule(void) {
	process_t* p;
	int	 greatest_ticks = 0;

	while (!greatest_ticks) {
		for (p = process_table; p < process_table + NR_TASKS; p++) {
			if (p->ticks > greatest_ticks) {
				greatest_ticks = p->ticks;
				process_ready = p;
			}
		}

		if (!greatest_ticks) {
			for (p = process_table; p < process_table + NR_TASKS; p++) {
				p->ticks = p->priority;
			}
		}
	}
}

/*======================================================================*
  sys_get_ticks
  *======================================================================*/
int sys_get_ticks(void) {
	return g_ticks;
}
