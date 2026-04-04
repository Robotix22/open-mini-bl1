CROSS = aarch64-linux-gnu-

CC      = $(CROSS)gcc
OBJCOPY = $(CROSS)objcopy

CFLAGS  = -ffreestanding -nostdlib -nostartfiles -Os -g -Iinclude

OBJS = start.o main.o bl1_info.o bootrom_funcs.o epbl_checks.o epbl_info.o pmu.o string.o

all: bl1.elf bl1.bin

start.o: start.S
	$(CC) $(CFLAGS) -c $< -o $@

main.o: main.c
	$(CC) $(CFLAGS) -c $< -o $@

bl1.elf: $(OBJS)
	$(CC) $(CFLAGS) -T linker.ld $(OBJS) -o $@

bl1.bin: bl1.elf
	$(OBJCOPY) -O binary $< $@
	truncate -s 9920 $@
	cat bl1-footer >> $@

clean:
	rm -f *.o *.elf *.bin
