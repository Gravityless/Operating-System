#include "lib.h"
#include "types.h"



int uEntry(void) {
	uint16_t selector;
	//uint16_t selector = 16;
	asm volatile("movw %%ss, %0":"=m"(selector)); // XXX necessary or not, iret may reset ds in QEMU
	asm volatile("movw %%ax, %%ds"::"a"(selector));
	
	// exec(261, 30);
	int ret = fork();
	int i = 8;
	if (ret == 0) {
		while( i != 0) {
			i--;
			printf("Child Process: Pong %d, %d;\n", ret, i);
			sleep(100);
		}
		exec(231, 30);
		exit();
	}
	else if (ret != -1) {
		while( i != 0) {
			i--;
			printf("Father Process: Ping %d, %d;\n", ret, i);
			sleep(100);
		}
		exit();
	}

	return 0;
}
