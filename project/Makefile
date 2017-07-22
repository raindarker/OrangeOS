##################################################
# Makefile
##################################################

BOOT:=boot.asm
LDR:=loader.asm
KERNEL:=kernel.asm
BOOT_BIN:=$(subst .asm,.bin,$(BOOT))
LDR_BIN:=$(subst .asm,.bin,$(LDR))
KERNEL_BIN:=$(subst .asm,.bin,$(KERNEL))

IMG:=OrangeOS.img
FLOPPY:=/mnt/floppy/

.PHONY : default

default : $(BOOT_BIN) $(LDR_BIN) $(KERNEL_BIN)
	@cp a.img $(IMG)
	@dd if=$(BOOT_BIN) of=$(IMG) bs=512 count=1 conv=notrunc
	@sudo mount -o loop $(IMG) $(FLOPPY)
	@sudo cp $(LDR_BIN) $(FLOPPY) -v
	@sudo cp $(KERNEL_BIN) $(FLOPPY) -v
	@sudo umount $(FLOPPY)

$(BOOT_BIN) : $(BOOT)
	@nasm $< -o $@

$(LDR_BIN) : $(LDR)
	@nasm $< -o $@

$(KERNEL_BIN) : $(KERNEL)
	nasm -f elf -o $(subst .asm,.o,$(KERNEL)) $<
	ld -s -o $@ $(subst .asm,.o,$(KERNEL))

clean:
	@rm -f *.o
	@rm -f *.bin
	@rm -f *.com
	@rm -f $(IMG)
	@echo "make clean done!"
