/*
 * File Name: protect.h
 * Author: Sphantix
 * Mail: hangxu@antiy.cn
 * Created Time: Mon 24 Jul 2017 10:38:49 AM CST
 */

#ifndef __ORANGE_PROTECT_H__
#define __ORANGE_PROTECT_H__
#include "types.h"

/* 存储段描述符/系统段描述符 */
typedef struct s_descriptor		/* 共 8 个字节 */
{
	u16	limit_low;		/* Limit */
	u16	base_low;		 /* Base */
	u8	base_mid;		 /* Base */
	u8	attr1;			 /* P(1) DPL(2) DT(1) TYPE(4) */
	u8	limit_high_attr2;	/* G(1) D(1) 0(1) AVL(1) LimitHigh(4) */
	u8	base_high;		/* Base */
} DESCRIPTOR;


#endif /* __ORANGE_PROTECT_H__ */
