/*
 * File Name: tty.c
 * Author: Sphantix
 * Mail: hangxu@antiy.cn
 * Created Time: Wed 09 Aug 2017 08:11:02 PM CST
 */

#include "types.h"
#include "tty.h"
#include "const.h"
#include "console.h"
#include "proc.h"
#include "proto.h"
#include "keyboard.h"

extern int           g_disp_pos;
extern int           g_re_enter;
extern process_t*    process_ready;
extern int           g_current_console;
extern tty_t         g_tty_table[NR_CONSOLES];
extern process_t     process_table[NR_TASKS + NR_PROCS];
extern console_t     g_console_table[NR_CONSOLES];

#define TTY_FIRST    (g_tty_table)
#define TTY_END      (g_tty_table + NR_CONSOLES)

static void init_tty(tty_t* tty) {
    tty->count = 0;
    tty->head = tty->tail = tty->buf;

    init_screen(tty);
}

static void tty_do_read(tty_t* tty) {
    if (is_current_console(tty->console)) {
        keyboard_read(tty);
    }
}

static void tty_do_write(tty_t* tty) {
    if (tty->count) {
        char c = *(tty->tail);
        tty->tail++;
        if (tty->tail == tty->buf + TTY_IN_BYTES) {
            tty->tail = tty->buf;
        }
        tty->count--;

        out_char(tty->console, c);
    }
}

void task_tty(void) {
    tty_t* tty;

    init_keyboard();

    for (tty = TTY_FIRST; tty < TTY_END; tty++) {
        init_tty(tty);
    }

    select_console(0);

    while(1) {
        for (tty = TTY_FIRST; tty < TTY_END; tty++) {
            tty_do_read(tty);
            tty_do_write(tty);
        }
    }
}

static void put_key(tty_t* tty, u32 key) {
	if (tty->count < TTY_IN_BYTES) {
		*(tty->head) = key;
		tty->head++;
		if (tty->head == tty->buf + TTY_IN_BYTES) {
			tty->head = tty->buf;
		}
		tty->count++;
	}
}

void in_process(tty_t* tty, u32 key) {
    char output[2] = {'\0', '\0'};

    if (!(key & FLAG_EXT)) {
        if (tty->count < TTY_IN_BYTES) {
            *(tty->head) = key;
            tty->head++;
            if (tty->head == tty->buf + TTY_IN_BYTES) {
                tty->head = tty->buf;
            }
            tty->count++;
        }
    } else {
        int raw_code = key & MASK_RAW;
        switch(raw_code) {
        case ENTER:
			put_key(tty, '\n');
            break;
        case BACKSPACE:
			put_key(tty, '\b');
            break;
        case UP:
            if ((key & FLAG_SHIFT_L) || (key & FLAG_SHIFT_R)) {
                scroll_screen(tty->console, SCR_DN);
            }
            break;
        case DOWN:
            if ((key & FLAG_SHIFT_L) || (key & FLAG_SHIFT_R)) {
                scroll_screen(tty->console, SCR_UP);
            }
            break;
        case F1:
        case F2:
        case F3:
        case F4:
        case F5:
        case F6:
        case F7:
        case F8:
        case F9:
        case F10:
        case F11:
        case F12:
            if ((key & FLAG_ALT_L) || (key & FLAG_ALT_R)) {
                select_console(raw_code - F3);
            } else {
                if (raw_code == F12) {
                    disable_interrupt();
                    dump_process(process_table + 4);
                    for(;;);
                }
            }
            break;
        default:
            break;
        }
    }
}

void tty_write(tty_t* tty, char* buf, int len) {
    char* c = buf;
    int i = len;

    while (i) {
        out_char(tty->console, *c++);
        i--;
    }
}

int sys_printx(int _unused1, int _unused2, char* s, process_t* process) {
    const char* p;
    char ch;

    char re_enter_err[] = "? g_reenter is incorrect for unknown reason";
    re_enter_err[0] = MAG_CH_PANIC;

	/**
	 * @note Code in both Ring 0 and Ring 1~3 may invoke printx().
	 * If this happens in Ring 0, no linear-physical address mapping
	 * is needed.
	 *
	 * @attention The value of `g_re_enter' is tricky here. When
	 *   -# printx() is called in Ring 0
	 *      - g_re_enter > 0. When code in Ring 0 calls printx(),
	 *        an `interrupt re-enter' will occur (printx() generates
	 *        a software interrupt). Thus `g_re_enter' will be increased
	 *        by `kernel.asm::save' and be greater than 0.
	 *   -# printx() is called in Ring 1~3
	 *      - g_re_enter == 0.
	 */
	if (g_re_enter == 0)  /* printx() called in Ring<1~3> */
		p = va2la(proc2pid(process), s);
	else if (g_re_enter > 0) /* printx() called in Ring<0> */
		p = s;
	else	/* this should NOT happen */
		p = re_enter_err;

	/**
	 * @note if assertion fails in any TASK, the system will be halted;
	 * if it fails in a USER PROC, it'll return like any normal syscall
	 * does.
	 */
	if ((*p == MAG_CH_PANIC) ||
	    (*p == MAG_CH_ASSERT && process_ready < &process_table[NR_TASKS])) {
		disable_interrupt();
		char* v = (char*)V_MEM_BASE;
		const char* q = p + 1; /* +1: skip the magic char */

		while (v < (char* )(V_MEM_BASE + V_MEM_SIZE)) {
			*v++ = *q++;
			*v++ = RED_CHAR;
			if (!*q) {
				while (((int)v - V_MEM_BASE) % (SCREEN_WIDTH * 16)) {
					/* *v++ = ' '; */
					v++;
					*v++ = GRAY_CHAR;
				}
				q = p + 1;
			}
		}

		__asm__ __volatile__("hlt");
	}

	while ((ch = *p++) != 0) {
		if (ch == MAG_CH_PANIC || ch == MAG_CH_ASSERT)
			continue; /* skip the magic char */

		out_char(g_tty_table[process->tty].console, ch);
	}

    return 0;
}

void dump_process(process_t* p) {
	int i;
	int text_color = MAKE_COLOR(GREEN, RED);
	char info[STR_DEFAULT_LEN];

	int dump_len = sizeof(process_t);

	out_byte(CRTC_ADDR_REG, START_ADDR_H);
	out_byte(CRTC_DATA_REG, 0);
	out_byte(CRTC_ADDR_REG, START_ADDR_L);
	out_byte(CRTC_DATA_REG, 0);

	sprintf(info, "byte dump of process_table[%d]:\n", p - process_table); disp_color_str(info, text_color);
	for (i = 0; i < dump_len; i++) {
		sprintf(info, "%x.", ((unsigned char *)p)[i]);
		disp_color_str(info, text_color);
	}

	/* printl("^^"); */

	disp_color_str("\n\n", text_color);
	sprintf(info, "ANY: 0x%x.\n", ANY); disp_color_str(info, text_color);
	sprintf(info, "NO_TASK: 0x%x.\n", NO_TASK); disp_color_str(info, text_color);
	disp_color_str("\n", text_color);

	sprintf(info, "ldt_sel: 0x%x.  ", p->ldt_sel); disp_color_str(info, text_color);
	sprintf(info, "ticks: 0x%x.  ", p->ticks); disp_color_str(info, text_color);
	sprintf(info, "priority: 0x%x.  ", p->priority); disp_color_str(info, text_color);
	sprintf(info, "pid: 0x%x.  ", p->pid); disp_color_str(info, text_color);
	sprintf(info, "name: %s.  ", p->name); disp_color_str(info, text_color);
	disp_color_str("\n", text_color);
	sprintf(info, "flags: 0x%x.  ", p->flags); disp_color_str(info, text_color);
	sprintf(info, "recvfrom: 0x%x.  ", p->recvfrom); disp_color_str(info, text_color);
	sprintf(info, "sendto: 0x%x.  ", p->sendto); disp_color_str(info, text_color);
	sprintf(info, "tty: 0x%x.  ", p->tty); disp_color_str(info, text_color);
	disp_color_str("\n", text_color);
	sprintf(info, "has_interrupt_msg: 0x%x.  ", p->has_interrupt_msg); disp_color_str(info, text_color);
}


void dump_msg(const char * title, message_t* msg) {
	int packed = 0;
	printl("{%s}<0x%x>{%ssrc:%s(%d),%stype:%d,%s(0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x)%s}%s",  //, (0x%x, 0x%x, 0x%x)}",
	       title,
	       (int)msg,
	       packed ? "" : "\n        ",
	       process_table[msg->source].name,
	       msg->source,
	       packed ? " " : "\n        ",
	       msg->type,
	       packed ? " " : "\n        ",
	       msg->u.m3.m3i1,
	       msg->u.m3.m3i2,
	       msg->u.m3.m3i3,
	       msg->u.m3.m3i4,
	       (int)msg->u.m3.m3p1,
	       (int)msg->u.m3.m3p2,
	       packed ? "" : "\n",
	       packed ? "" : "\n"/* , */
		);
}
