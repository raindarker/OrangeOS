#########################
# Makefile for OrangeOS #
#########################

# Entry point of OrangeOS
# It must have the same value with 'KernelEntryPointPhyAddr' in load.inc!
ENTRYPOINT	= 0x30400

# Offset of entry point in kernel file
# It depends on ENTRYPOINT
ENTRYOFFSET	= 0x400

# Programs, flags, etc.
ASM		= nasm
DASM	= ndisasm
CC		= gcc
LD		= ld
ASMBFLAGS	= -I ./ -I boot/include/
ASMKFLAGS	= -I include/ -f elf
CFLAGS		= -m32 -I include/ -c -fno-builtin -fno-stack-protector
LDFLAGS		= -melf_i386 -s -Ttext $(ENTRYPOINT)
DASMFLAGS	= -u -o $(ENTRYPOINT) -e $(ENTRYOFFSET)

# This Program
ORANGESBOOT	= boot/boot.bin boot/loader.bin
ORANGESKERNEL = kernel.bin
OBJS = kernel/kernel.o kernel/syscall.o kernel/keyboard.o \
       kernel/console.o kernel/tty.o kernel/start.o kernel/main.o \
       kernel/proc.o kernel/clock.o kernel/printf.o kernel/vsprintf.o \
       kernel/i8259.o kernel/global.o kernel/protect.o kernel/systask.o \
       lib/klib.o lib/misc.o lib/kliba.o lib/string.o
DASMOUTPUT = kernel.bin.asm
IMAGE:=OrangeOS.img

# All Phony Targets
.PHONY : everything image clean disasm all buildimg

# Default starting position
everything : $(ORANGESBOOT) $(ORANGESKERNEL)

all : clean everything

image : all buildimg

clean :
	@rm -f $(OBJS) $(ORANGESBOOT) $(ORANGESKERNEL) $(IMAGE)

disasm :
	$(DASM) $(DASMFLAGS) $(ORANGESKERNEL) > $(DASMOUTPUT)

# We assume that "a.img" exists in current folder
buildimg :
	@cp a.img $(IMAGE)
	@dd if=boot/boot.bin of=$(IMAGE) bs=512 count=1 conv=notrunc
	@sudo mount -o loop $(IMAGE) /mnt/floppy/
	@sudo cp -fv boot/loader.bin /mnt/floppy/
	@sudo cp -fv kernel.bin /mnt/floppy
	@sudo umount /mnt/floppy

boot/boot.bin : boot/boot.asm boot/include/load.inc boot/include/fat12hdr.inc
	@$(ASM) $(ASMBFLAGS) -o $@ $<

boot/loader.bin : boot/loader.asm boot/include/load.inc \
			boot/include/fat12hdr.inc boot/include/pm.inc
	@$(ASM) $(ASMBFLAGS) -o $@ $<

$(ORANGESKERNEL) : $(OBJS)
	@$(LD) $(LDFLAGS) -o $(ORANGESKERNEL) $(OBJS)

kernel/kernel.o : kernel/kernel.asm include/sconst.inc
	@$(ASM) $(ASMKFLAGS) -o $@ $<

kernel/syscall.o : kernel/syscall.asm include/sconst.inc
	@$(ASM) $(ASMKFLAGS) -o $@ $<

kernel/clock.o: kernel/clock.c include/proto.h include/types.h
	@$(CC) $(CFLAGS) -o $@ $<

kernel/printf.o: kernel/printf.c include/types.h
	@$(CC) $(CFLAGS) -o $@ $<

kernel/vsprintf.o: kernel/vsprintf.c include/types.h include/string.h
	@$(CC) $(CFLAGS) -o $@ $<

kernel/keyboard.o: kernel/keyboard.c include/types.h include/const.h \
	include/proto.h include/types.h
	@$(CC) $(CFLAGS) -o $@ $<

kernel/tty.o: kernel/tty.c include/types.h include/const.h include/proto.h \
	include/types.h include/tty.h include/console.h include/keyboard.h
	@$(CC) $(CFLAGS) -o $@ $<

kernel/console.o: kernel/console.c include/types.h include/const.h \
	include/console.h include/proto.h include/types.h include/tty.h \
	include/console.h
	@$(CC) $(CFLAGS) -o $@ $<

kernel/start.o: kernel/start.c include/types.h include/const.h include/protect.h \
	include/types.h include/proto.h include/string.h
	@$(CC) $(CFLAGS) -o $@ $<

kernel/main.o: kernel/main.c include/proto.h include/types.h include/proc.h \
	include/const.h include/protect.h include/protect.h include/const.h
	@$(CC) $(CFLAGS) -o $@ $<

kernel/proc.o: kernel/proc.c include/types.h include/const.h include/proto.h \
	include/types.h include/proc.h include/const.h include/protect.h
	@$(CC) $(CFLAGS) -o $@ $<

kernel/i8259.o: kernel/i8259.c include/types.h include/const.h include/protect.h \
	include/types.h include/proto.h
	@$(CC) $(CFLAGS) -o $@ $<

kernel/global.o: kernel/global.c include/types.h include/const.h include/protect.h \
	include/types.h include/proto.h include/proc.h include/const.h include/protect.h
	@$(CC) $(CFLAGS) -o $@ $<

kernel/protect.o: kernel/protect.c include/types.h include/const.h include/proc.h \
	include/const.h include/protect.h include/types.h include/protect.h include/proto.h
	@$(CC) $(CFLAGS) -o $@ $<

kernel/systask.o: kernel/systask.c
	@$(CC) $(CFLAGS) -o $@ $<

lib/klib.o: lib/klib.c include/types.h include/const.h include/protect.h include/types.h \
	include/proto.h include/string.h
	@$(CC) $(CFLAGS) -o $@ $<

lib/misc.o: lib/misc.c
	@$(CC) $(CFLAGS) -o $@ $<

lib/kliba.o : lib/kliba.asm
	@$(ASM) $(ASMKFLAGS) -o $@ $<

lib/string.o : lib/string.asm
	@$(ASM) $(ASMKFLAGS) -o $@ $<
