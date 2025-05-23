CROSS_COMPILE=aarch64-linux-gnu-
CC=$(CROSS_COMPILE)gcc
LD=$(CROSS_COMPILE)ld
OBJCOPY=$(CROSS_COMPILE)objcopy

# Include paths
INCLUDES=-I. -Ikernel -Idrivers

CFLAGS=-nostdlib -nostartfiles -ffreestanding -O2 -Wall -Wextra $(INCLUDES)
LDFLAGS=-T linker.ld

# Source files
BOOT_SOURCES = $(wildcard boot/*.S)
KERNEL_SOURCES = $(wildcard kernel/*.c) $(wildcard kernel/*/*.c)
DRIVER_SOURCES = $(wildcard drivers/*.c) $(wildcard drivers/*/*.c)

SOURCES = $(BOOT_SOURCES) $(KERNEL_SOURCES) $(DRIVER_SOURCES)
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

# Show build information
info:
	@echo "SAGE OS Build Information"
	@echo "------------------------"
	@echo "Compiler: $(CC)"
	@echo "Linker: $(LD)"
	@echo "Object Copy: $(OBJCOPY)"
	@echo "CFLAGS: $(CFLAGS)"
	@echo "LDFLAGS: $(LDFLAGS)"
	@echo "Source files: $(words $(SOURCES))"
	@echo "Object files: $(words $(OBJECTS))"
