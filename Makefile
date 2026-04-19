CC      = clang
OBJCOPY = llvm-objcopy

CFLAGS  = --target=aarch64-linux-gnu -ffreestanding -nostdlib -Os -g -Iinclude
LDFLAGS = --target=aarch64-linux-gnu -ffreestanding -nostdlib -T arch/arm64/linker.ld

OBJS = arch/arm64/start.o main.o bl1/bl1_info.o bootrom/bootrom_funcs.o bl31/bl31_checks.o bl31/bl31_info.o bl31/bl31_loader.o subsystems/gpio.o subsystems/pmu.o libc/string.o

all: bl1.elf bl1.bin

%.o: %.S
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

bl1.elf: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@

bl1.bin: bl1.elf
	$(OBJCOPY) -O binary $< $@
	truncate -s 7152 $@
	cat misc/bl1-footer >> $@

clean:
	rm -f $(OBJS) bl1.elf bl1.bin
