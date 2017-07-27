/*
 * File Name: start.c
 * Author: Sphantix
 * Mail: hangxu@antiy.cn
 * Created Time: Mon 24 Jul 2017 10:42:06 AM CST
 */


#include "types.h"
#include "const.h"
#include "protect.h"
#include "proto.h"
#include "string.h"

extern u8           gdt_ptr[6];	/* 0~15:Limit  16~47:Base */
extern descriptor_t gdt[GDT_SIZE];
extern u8           idt_ptr[6];	/* 0~15:Limit  16~47:Base */
extern gate_t       idt[IDT_SIZE];


/*======================================================================*
  cstart
  *======================================================================*/
void cstart() {
    disp_str("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"
             "-----\"cstart\" begins-----\n");

    /* 将 LOADER 中的 GDT 复制到新的 GDT 中 */
    memcpy(&gdt,                                /* New GDT */
           (void*)(*((u32*)(&gdt_ptr[2]))),     /* Base  of Old GDT */
           *((u16*)(&gdt_ptr[0])) + 1           /* Limit of Old GDT */
           );
    /* gdt_ptr[6] 共 6 个字节：0~15:Limit  16~47:Base。用作 sgdt/lgdt 的参数。*/
    u16* gdt_limit = (u16*)(&gdt_ptr[0]);
    u32* gdt_base  = (u32*)(&gdt_ptr[2]);
    *gdt_limit = GDT_SIZE * sizeof(descriptor_t) - 1;
    *gdt_base  = (u32)&gdt;

    /* idt_ptr[6] 共 6 个字节：0~15:Limit  16~47:Base。用作 sidt/lidt 的参数。*/
    u16* idt_limit = (u16*)(&idt_ptr[0]);
    u32* idt_base  = (u32*)(&idt_ptr[2]);
    *idt_limit = IDT_SIZE * sizeof(gate_t) - 1;
    *idt_base  = (u32)&idt;

    init_prot();

    disp_str("-----\"cstart\" ends-----\n");
}
