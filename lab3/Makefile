QEMU = qemu-system-i386

os.img:
	@cd bootloader; make
	@cd kernel; make
	@cd app; make
	@cd app_print; make
	@cd app_test; make
	cat bootloader/bootloader.bin kernel/kMain.elf app/uMain.elf app_print/app_print.elf app_test/app_test.elf > os.img
	#   bootloader从0扇区开始，kernel从1扇区开始，app1从201扇区开始，app2从231扇区开始。
play: os.img
	$(QEMU) -serial stdio os.img

debug: os.img
	$(QEMU) -serial stdio -s -S os.img
gdb:
	gdb -n -x ./.gdbconf/.gdbinit



clean:
	@cd bootloader; make clean
	@cd kernel; make clean
	@cd app; make clean
	@cd app_print; make clean
	@cd app_test; make clean
	rm -f os.img
