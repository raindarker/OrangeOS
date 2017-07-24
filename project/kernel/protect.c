#include "types.h"
#include "const.h"
#include "protect.h"
#include "proto.h"

extern int         disp_pos;
extern u8          gdt_ptr[6];	/* 0~15:Limit  16~47:Base */
extern DESCRIPTOR	gdt[GDT_SIZE];
extern u8          idt_ptr[6];	/* 0~15:Limit  16~47:Base */
extern GATE        idt[IDT_SIZE];

/* 本文件内函数声明 */
static void init_idt_desc(unsigned char vector, u8 desc_type,
                          int_handler handler, unsigned char privilege);

/* 中断处理函数 */
extern void divide_error();
extern void single_step_exception();
extern void nmi();
extern void breakpoint_exception();
extern void overflow();
extern void bounds_check();
extern void inval_opcode();
extern void copr_not_available();
extern void double_fault();
extern void copr_seg_overrun();
extern void inval_tss();
extern void segment_not_present();
extern void stack_exception();
extern void general_protection();
extern void page_fault();
extern void copr_error();
extern void hwint00();
extern void hwint01();
extern void hwint02();
extern void hwint03();
extern void hwint04();
extern void hwint05();
extern void hwint06();
extern void hwint07();
extern void hwint08();
extern void hwint09();
extern void hwint10();
extern void hwint11();
extern void hwint12();
extern void hwint13();
extern void hwint14();
extern void hwint15();


/*======================================================================*
  init_prot
  *======================================================================*/
void init_prot() {
    init_8259A();

    // 全部初始化成中断门(没有陷阱门)
    init_idt_desc(INT_VECTOR_DIVIDE, DA_386IGate, divide_error, PRIVILEGE_KRNL);

    init_idt_desc(INT_VECTOR_DEBUG, DA_386IGate, single_step_exception, PRIVILEGE_KRNL);

    init_idt_desc(INT_VECTOR_NMI, DA_386IGate, nmi, PRIVILEGE_KRNL);

    init_idt_desc(INT_VECTOR_BREAKPOINT, DA_386IGate, breakpoint_exception,	PRIVILEGE_USER);

    init_idt_desc(INT_VECTOR_OVERFLOW, DA_386IGate, overflow, PRIVILEGE_USER);

    init_idt_desc(INT_VECTOR_BOUNDS, DA_386IGate, bounds_check, PRIVILEGE_KRNL);

    init_idt_desc(INT_VECTOR_INVAL_OP, DA_386IGate, inval_opcode, PRIVILEGE_KRNL);

    init_idt_desc(INT_VECTOR_COPROC_NOT, DA_386IGate, copr_not_available, PRIVILEGE_KRNL);

    init_idt_desc(INT_VECTOR_DOUBLE_FAULT, DA_386IGate, double_fault, PRIVILEGE_KRNL);

    init_idt_desc(INT_VECTOR_COPROC_SEG, DA_386IGate, copr_seg_overrun, PRIVILEGE_KRNL);

    init_idt_desc(INT_VECTOR_INVAL_TSS,	DA_386IGate, inval_tss, PRIVILEGE_KRNL);

    init_idt_desc(INT_VECTOR_SEG_NOT, DA_386IGate, segment_not_present, PRIVILEGE_KRNL);

    init_idt_desc(INT_VECTOR_STACK_FAULT, DA_386IGate, stack_exception, PRIVILEGE_KRNL);

    init_idt_desc(INT_VECTOR_PROTECTION, DA_386IGate, general_protection, PRIVILEGE_KRNL);

    init_idt_desc(INT_VECTOR_PAGE_FAULT, DA_386IGate, page_fault, PRIVILEGE_KRNL);

    init_idt_desc(INT_VECTOR_COPROC_ERR, DA_386IGate, copr_error, PRIVILEGE_KRNL);

    init_idt_desc(INT_VECTOR_IRQ0 + 0, DA_386IGate, hwint00, PRIVILEGE_KRNL);

    init_idt_desc(INT_VECTOR_IRQ0 + 1, DA_386IGate, hwint01, PRIVILEGE_KRNL);

    init_idt_desc(INT_VECTOR_IRQ0 + 2, DA_386IGate, hwint02, PRIVILEGE_KRNL);

    init_idt_desc(INT_VECTOR_IRQ0 + 3, DA_386IGate, hwint03, PRIVILEGE_KRNL);

    init_idt_desc(INT_VECTOR_IRQ0 + 4, DA_386IGate, hwint04, PRIVILEGE_KRNL);

    init_idt_desc(INT_VECTOR_IRQ0 + 5, DA_386IGate, hwint05, PRIVILEGE_KRNL);

    init_idt_desc(INT_VECTOR_IRQ0 + 6, DA_386IGate, hwint06, PRIVILEGE_KRNL);

    init_idt_desc(INT_VECTOR_IRQ0 + 7, DA_386IGate, hwint07, PRIVILEGE_KRNL);

    init_idt_desc(INT_VECTOR_IRQ8 + 0, DA_386IGate, hwint08, PRIVILEGE_KRNL);

    init_idt_desc(INT_VECTOR_IRQ8 + 1, DA_386IGate, hwint09, PRIVILEGE_KRNL);

    init_idt_desc(INT_VECTOR_IRQ8 + 2, DA_386IGate, hwint10, PRIVILEGE_KRNL);

    init_idt_desc(INT_VECTOR_IRQ8 + 3, DA_386IGate, hwint11, PRIVILEGE_KRNL);

    init_idt_desc(INT_VECTOR_IRQ8 + 4, DA_386IGate, hwint12, PRIVILEGE_KRNL);

    init_idt_desc(INT_VECTOR_IRQ8 + 5, DA_386IGate, hwint13, PRIVILEGE_KRNL);

    init_idt_desc(INT_VECTOR_IRQ8 + 6, DA_386IGate, hwint14, PRIVILEGE_KRNL);

    init_idt_desc(INT_VECTOR_IRQ8 + 7, DA_386IGate, hwint15, PRIVILEGE_KRNL);
}

/*======================================================================*
  init_idt_desc
  *----------------------------------------------------------------------*
  初始化 386 中断门
  *======================================================================*/
static void init_idt_desc(unsigned char vector, u8 desc_type,
                          int_handler handler, unsigned char privilege) {
    GATE *	p_gate	= &idt[vector];
    u32	base	= (u32)handler;
    p_gate->offset_low	= base & 0xFFFF;
    p_gate->selector	= SELECTOR_KERNEL_CS;
    p_gate->dcount		= 0;
    p_gate->attr		= desc_type | (privilege << 5);
    p_gate->offset_high	= (base >> 16) & 0xFFFF;
}

/*======================================================================*
  exception_handler
  *----------------------------------------------------------------------*
  异常处理
  *======================================================================*/
void exception_handler(int vec_no,int err_code,int eip,int cs,int eflags) {
    int i;
    int text_color = 0x74; /* 灰底红字 */

    char * err_msg[] = {"#DE Divide Error",
                        "#DB RESERVED",
                        "--  NMI Interrupt",
                        "#BP Breakpoint",
                        "#OF Overflow",
                        "#BR BOUND Range Exceeded",
                        "#UD Invalid Opcode (Undefined Opcode)",
                        "#NM Device Not Available (No Math Coprocessor)",
                        "#DF Double Fault",
                        "    Coprocessor Segment Overrun (reserved)",
                        "#TS Invalid TSS",
                        "#NP Segment Not Present",
                        "#SS Stack-Segment Fault",
                        "#GP General Protection",
                        "#PF Page Fault",
                        "--  (Intel reserved. Do not use.)",
                        "#MF x87 FPU Floating-Point Error (Math Fault)",
                        "#AC Alignment Check",
                        "#MC Machine Check",
                        "#XF SIMD Floating-Point Exception"
    };

    /* 通过打印空格的方式清空屏幕的前五行，并把 disp_pos 清零 */
    disp_pos = 0;
    for(i=0;i<80*5;i++) {
        disp_str(" ");
    }
    disp_pos = 0;

    disp_color_str("Exception! --> ", text_color);
    disp_color_str(err_msg[vec_no], text_color);
    disp_color_str("\n\n", text_color);
    disp_color_str("EFLAGS:", text_color);
    disp_int(eflags);
    disp_color_str("CS:", text_color);
    disp_int(cs);
    disp_color_str("EIP:", text_color);
    disp_int(eip);

    if(err_code != 0xFFFFFFFF) {
        disp_color_str("Error code:", text_color);
        disp_int(err_code);
    }
}
