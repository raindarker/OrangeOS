default: boot.bin a.img
	@cp a.img OrangeOS.img
	@dd if=boot.bin of=OrangeOS.img bs=512 count=1 conv=notrunc

boot.bin:
	@nasm pmtest1.asm -o pmtest1.bin
	@mv pmtest1.bin boot.bin

clean:
	@rm -f *.o
	@rm -f *.bin
	@rm -f OrangeOS.img
	@echo "make clean done!"
