void bootMain(void) {
	int i = 0;
	unsigned int elf = 0x100000;
	void (*kMainEntry)(void);
	kMainEntry = (void(*)(void))0x100000;

	for (i = 0; i < 200; i++) {
		readSect((void*)(elf + i*512), 1+i);
	}

	// TODO: 填写kMainEntry、phoff、offset...... 然后加载Kernel（可以参考NEMU的某次lab）
	ELFHeader *eh = (ELFHeader*)elf;
    kMainEntry = (void(*)(void))(eh->entry);

	// get the first program header
    ProgramHeader *ph = (ProgramHeader*)(elf + eh->phoff);
	
	// iterate all headers
	for(i = 0; i < eh->phnum; i++, ph++) {
		// check weither it is LOAD
		if (ph->type == 0x1) {
			// loading filesz
			for (int j = 0; j < ph->filesz; j++) {
				*(unsigned char *)(ph->vaddr + j) = *(unsigned char *)(elf + j + ph->off);
			}

			//loading other memsz 
			for (int j = ph->filesz; j < ph->memsz; j++) {
				*(unsigned char *)(ph->vaddr + j) = 0;
			}
		}
	}

	kMainEntry();
}