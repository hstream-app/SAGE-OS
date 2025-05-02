CROSS_COMPILE=aarch64-none-elf-
CC=$(CROSS_COMPILE)gcc
LD=$(CROSS_COMPILE)ld
OBJCOPY=$(CROSS_COMPILE)objcopy

CFLAGS=-nostdlib -nostartfiles -ffreestanding -O2 -Wall -Wextra
LDFLAGS=-T linker.ld

SOURCES = $(wildcard boot/*.S kernel/*.c drivers/*.c)
OBJECTS = $(SOURCES:.c=.o)
OBJECTS := $(OBJECTS:.S=.o)

all: kernel8.img

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.S
	$(CC) $(CFLAGS) -c $< -o $@

kernel8.img: $(OBJECTS)
	$(LD) $(LDFLAGS) -o kernel.elf $(OBJECTS)
	$(OBJCOPY) -O binary kernel.elf $@

clean:
	rm -f $(OBJECTS) kernel.elf kernel8.img
