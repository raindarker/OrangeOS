##################################################
# Makefile
##################################################

BOOT:=boot.asm
LDR:=loader.asm
BOOT_BIN:=$(subst .asm,.bin,$(BOOT))
LDR_BIN:=$(subst .asm,.bin,$(LDR))

IMG:=OrangeOS.img
FLOPPY:=/mnt/floppy/

.PHONY : default

default : $(BOOT_BIN) $(LDR_BIN)
	@cp a.img $(IMG)
	@dd if=$(BOOT_BIN) of=$(IMG) bs=512 count=1 conv=notrunc
	@sudo mount -o loop $(IMG) $(FLOPPY)
	@sudo cp $(LDR_BIN) $(FLOPPY) -v
	@sudo umount $(FLOPPY)

$(BOOT_BIN) : $(BOOT)
	@nasm $< -o $@

$(LDR_BIN) : $(LDR)
	@nasm $< -o $@

clean:
	@rm -f *.o
	@rm -f *.bin
	@rm -f *.com
	@rm -f $(IMG)
	@echo "make clean done!"
