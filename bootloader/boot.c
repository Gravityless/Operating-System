#include "boot.h"

#define SECTSIZE 512





void bootMain(void) {
	int i = 0;
	int phoff = 0x34;
	int phnum = 0x1;
	int memsz = 0;
	int vaddr = 0;
	int type = 0;
	int offset = 0x1000;
	unsigned int elf = 0x100000;
	void (*kMainEntry)(void);
	kMainEntry = (void(*)(void))0x100000;

	for (i = 0; i < 200; i++) {
		readSect((void*)(elf + i*512), 1+i);
	}

	// TODO: 填写kMainEntry、phoff、offset...... 然后加载Kernel（可以参考NEMU的某次lab）
	ELFHeader *eh = (ELFHeader*)elf;
    kMainEntry = (void(*)(void))(eh->entry);
    phoff = eh->phoff;
	phnum = eh->phnum;

	// get the first program header
    ProgramHeader *ph = (ProgramHeader*)(elf + phoff);
	
	// iterate all headers
	for(i = 0; i < phnum; i++, ph++) {
		offset = ph->off;
		memsz = ph->memsz;
		vaddr = ph->vaddr;
		type = ph->type;

		// check weither it is LOAD
		if (type == 0x1) {
			for (int j = 0; j < memsz; j++) {
				*(unsigned char *)(vaddr + j) = *(unsigned char *)(elf + j + offset);
			}
		}
	}

	kMainEntry();
}

void waitDisk(void) { // waiting for disk
	while((inByte(0x1F7) & 0xC0) != 0x40);
}

void readSect(void *dst, int offset) { // reading a sector of disk
	int i;
	waitDisk();
	outByte(0x1F2, 1);
	outByte(0x1F3, offset);
	outByte(0x1F4, offset >> 8);
	outByte(0x1F5, offset >> 16);
	outByte(0x1F6, (offset >> 24) | 0xE0);
	outByte(0x1F7, 0x20);

	waitDisk();
	for (i = 0; i < SECTSIZE / 4; i ++) {
		((int *)dst)[i] = inLong(0x1F0);
	}
}
