#include "types.h"
#include "utils.h"
#include "lib.h"

union DirEntry {
	uint8_t byte[128];
	struct {
		uint32_t inode;
		char name[64];
	};
};

typedef union DirEntry DirEntry;


int ls(char *destFilePath) {
	printf("ls %s\n", destFilePath);
	int fd = 0;
	int ret = 0;
	DirEntry *dirEntry = 0;
	uint8_t buffer[512 * 2];
	fd = open(destFilePath, O_READ | O_DIRECTORY);
	if (fd == -1)
		return -1;
	ret = read(fd, buffer, 512 * 2);
	if (ret != 0) {
		// TODO: ls
		// Hint: 使用 DIrEntry
		for (int j = 0; j < ret / sizeof(DirEntry); j++) {
			dirEntry = (DirEntry *)(buffer + j * sizeof(DirEntry));
			if (dirEntry->inode != 0)
				printf("Name: %s Inode: %d\n", dirEntry->name, dirEntry->inode);
		}
	}
	printf("\n");
	close(fd);
	return 0;
}

int cat(char *destFilePath) {
	printf("cat %s\n", destFilePath);
	int fd = 0;
	int ret = 0;
	uint8_t buffer[512 * 2];
	fd = open(destFilePath, O_READ);
	if (fd == -1)
		return -1;
	ret = read(fd, buffer, 512 * 2);
	if (ret != 0) {
		// TODO: cat
		//把内容读到buffer，输出...
		printf("%s\n", buffer);
	}
	close(fd);
	return 0;
}

int uEntry(void) {
	int fd = 0;
	int i = 0;
	char tmp = 0;
	
	ls("/");
	ls("/boot/");
	ls("/dev/");
	ls("/usr/");

	printf("create /usr/test and write alphabets to it\n");
	fd = open("/usr/test", O_WRITE | O_READ | O_CREATE);
	for (i = 0; i < 26; i ++) {
		tmp = (char)(i % 26 + 'A');
		write(fd, (uint8_t*)&tmp, 1);
	}
	close(fd);
	ls("/usr/");
	cat("/usr/test");
	printf("\n");
	printf("rm /usr/test\n");
	remove("/usr/test");
	ls("/usr/");
	printf("rmdir /usr/\n");
	remove("/usr");
	ls("/");
	// printf("create /usr/\n");
	// fd = open("/usr/", O_CREATE | O_DIRECTORY);
	// close(fd);
	// ls("/");
	
	exit();
	return 0;
}
