#default: boot.bin a.img
	#@cp a.img OrangeOS.img
	#@dd if=boot.bin of=OrangeOS.img bs=512 count=1 conv=notrunc

default:
	@nasm pmtest.asm -o pmtest.com
	@cp a.img pm.img
	@sudo mount -o loop pm.img /mnt/floppy
	@sudo cp pmtest.com /mnt/floppy
	@sudo umount /mnt/floppy

clean:
	@rm -f *.o
	@rm -f *.bin
	@rm -f *.com
	@rm pm.img
	@echo "make clean done!"
