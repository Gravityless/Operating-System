CC = gcc
LD = ld

CFLAGS = -m32 -march=i386 -static \
	 -fno-builtin -fno-stack-protector -fno-omit-frame-pointer \
	 -Wall -Werror -O2 -I../lib
LDFLAGS = -m elf_i386

UCFILES = $(shell find ./ -name "*.c")
LCFILES = $(shell find ../lib -name "*.c")
UOBJS = $(UCFILES:.c=.o) $(LCFILES:.c=.o)


app_test.bin: $(UOBJS)
	$(LD) $(LDFLAGS) -e main -Ttext 0x00000000 -o app_test.elf $(UOBJS)
	@../utils/genApp.pl app_test.elf

clean:
	rm -rf $(UOBJS) app_test.elf
