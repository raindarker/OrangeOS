/*
 * File Name: clock.h
 * Author: Sphantix
 * Mail: hangxu@antiy.cn
 * Created Time: Tue 22 Aug 2017 11:18:22 AM CST
 */

#ifndef __ORANGE_CLOCK_H__
#define __ORANGE_CLOCK_H__

void clock_handler(int irq);
int get_ticks(void);
void milli_delay(int milli_sec);
void init_clock(void);

#endif /* __ORANGE_CLOCK_H__ */
