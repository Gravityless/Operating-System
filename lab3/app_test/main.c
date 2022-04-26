#include "lib.h"
#include "types.h"

int main() {
	uint16_t selector;
	//uint16_t selector = 16;
	asm volatile("movw %%ss, %0":"=m"(selector)); // XXX necessary or not, iret may reset ds in QEMU
	asm volatile("movw %%ax, %%ds"::"a"(selector));
	
	int dishes = 10;
	int consumer = 1;
	int child = fork();
	if (child == 0) {
		while (dishes > 0) {
			printf("Consumer consume 1 dish\n");
			dishes--;
			printf("Last dishes: %d\n", dishes);
			sleep(50);
		}
		printf("No more dishes, leave\n");
		consumer--;
		exit();
	}
	else if (child != -1) {
		while (consumer > 0) {
        	printf("Producer provide: 1 dishes\n");
			dishes += 1;
			printf("Last dishes: %d\n", dishes);
			sleep(100);
		}
		printf("No consumer, leave\n");
		exit();
	}

	return 0;
}