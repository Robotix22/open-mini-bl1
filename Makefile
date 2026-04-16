ARCH = arm64
CROSS = aarch64-linux-gnu

CC      = clang --target=$(CROSS)
OBJCOPY = $(CROSS)-objcopy

CFLAGS  = -ffreestanding -Oz -flto -g -Iinclude -fno-pic
LDFLAGS = -nostdlib -nostartfiles -fuse-ld=lld

OBJS = arch/$(ARCH)/start.o main.o bl1/bl1_info.o bootrom/bootrom_funcs.o epbl/epbl_checks.o epbl/epbl_info.o epbl/epbl_loader.o subsystems/pmu.o libc/string.o

all: bl1.elf bl1.bin

start.o: start.S
	$(CC) $(CFLAGS) -c $< -o $@

main.o: main.c
	$(CC) $(CFLAGS) -c $< -o $@

bl1.elf: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -T arch/$(ARCH)/linker.ld $(OBJS) -o $@

bl1.bin: bl1.elf
	$(OBJCOPY) -O binary $< $@
	ls -l $@
	truncate -s 9920 $@
	cat misc/bl1-footer >> $@

clean:
	rm -f $(OBJS) bl1.elf bl1.bin
