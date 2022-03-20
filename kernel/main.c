#include "common.h"
#include "x86.h"
#include "device.h"

void kEntry(void) {
	// Interruption is disabled in bootloader
	initSerial();// initialize serial port
	putStr("Initialize Serial Successfully");
	putChar('\n');
	
	// TODO: 做一系列初始化
	// initialize idt
	initIdt();
	putStr("Initialize idt ");

	// iniialize 8259a
	initIntr();
	putStr("8259A ");

	// initialize gdt, tss
	initSeg();
	putStr("GDT TSS ");

	// initialize vga device
	initVga();
	putStr("VGA ");

	// initialize keyboard device
	initKeyTable();
	putStr("Keyboard");
	putChar('\n');

	loadUMain(); // load user program, enter user space

	while(1);
	assert(0);
}
