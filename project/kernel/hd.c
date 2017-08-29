/*
 * File Name: hd.c
 * Author: Sphantix
 * Mail: hangxu@antiy.cn
 * Created Time: Wed 23 Aug 2017 07:10:51 PM CST
 */

#include "hd.h"
#include "proc.h"
#include "proto.h"
#include "const.h"
#include "types.h"
#include "assert.h"

static u8 hd_status;
static u8 hdbuf[SECTOR_SIZE * 2];

void hd_handler(int irq) {
    hd_status = in_byte(REG_STATUS);

    inform_interrupt(TASK_HD);
}

void init_hd() {
    u8* DrivesNum = (u8 *)(0x475);
    printl("DrivesNum = %d.\n", *DrivesNum);
    assert(*DrivesNum);

    set_irq_handler(AT_WINI_IRQ, hd_handler);
    enable_irq(CASCADE_IRQ);
    enable_irq(AT_WINI_IRQ);
}

static int waitfor(int mask, int val, int timeout) {
	int t = get_ticks();

	while(((get_ticks() - t) * 1000 / HZ) < timeout) {
		if ((in_byte(REG_STATUS) & mask) == val)
			return 1;
    }

	return 0;
}

static void hd_cmd_out(hd_cmd_t* cmd) {
	/**
	 * For all commands, the host must first check if BSY=1,
	 * and should proceed no further unless and until BSY=0
	 */
	if (!waitfor(STATUS_BSY, 0, HD_TIMEOUT))
		panic("hd error.");

	/* Activate the Interrupt Enable (nIEN) bit */
	out_byte(REG_DEV_CTRL, 0);
	/* Load required parameters in the Command Block Registers */
	out_byte(REG_FEATURES, cmd->features);
	out_byte(REG_NSECTOR,  cmd->count);
	out_byte(REG_LBA_LOW,  cmd->lba_low);
	out_byte(REG_LBA_MID,  cmd->lba_mid);
	out_byte(REG_LBA_HIGH, cmd->lba_high);
	out_byte(REG_DEVICE,   cmd->device);
	/* Write the command code to the Command Register */
	out_byte(REG_CMD,     cmd->command);
}

static void interrupt_wait(void) {
	message_t msg;
	send_recv(RECEIVE, INTERRUPT, &msg);
}

static void print_identify_info(u16* hdinfo) {
	int i, k;
	char s[64];

	struct iden_info_ascii {
		int idx;
		int len;
		char* desc;
	} iinfo[] = {{10, 20, "HD SN"}, /* Serial number in ASCII */
                 {27, 40, "HD Model"} /* Model number in ASCII */ };

	for (k = 0; k < sizeof(iinfo)/sizeof(iinfo[0]); k++) {
		char * p = (char*)&hdinfo[iinfo[k].idx];
		for (i = 0; i < iinfo[k].len/2; i++) {
			s[i*2+1] = *p++;
			s[i*2] = *p++;
		}
		s[i*2] = 0;
		printl("%s: %s\n", iinfo[k].desc, s);
	}

	int capabilities = hdinfo[49];
	printl("LBA supported: %s\n",
	       (capabilities & 0x0200) ? "Yes" : "No");

	int cmd_set_supported = hdinfo[83];
	printl("LBA48 supported: %s\n",
	       (cmd_set_supported & 0x0400) ? "Yes" : "No");

	int sectors = ((int)hdinfo[61] << 16) + hdinfo[60];
	printl("HD size: %dMB\n", sectors * 512 / 1000000);
}

static void hd_identify(int drive) {
	hd_cmd_t cmd = {.device = MAKE_DEVICE_REG(0, drive, 0),
                    .command = ATA_IDENTIFY};

	hd_cmd_out(&cmd);
	interrupt_wait();
	port_read(REG_DATA, hdbuf, SECTOR_SIZE);

	print_identify_info((u16*)hdbuf);
}

void task_hd(void) {
    message_t msg;

    init_hd();

    while (1) {
        send_recv(RECEIVE, ANY, &msg);
        int src = msg.source;
        switch (msg.type) {
        case DEV_OPEN: {
            hd_identify(0);
            break;
        }
        default:
			dump_msg("HD driver::unknown msg", &msg);
			spin("FS::main_loop (invalid msg.type)");
            break;
        }
        send_recv(SEND, src, &msg);
    }
}
