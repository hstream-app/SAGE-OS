ARCH ?= aarch64
ifeq ($(ARCH),x86_64)
    CROSS_COMPILE=x86_64-linux-gnu-
else ifeq ($(ARCH),arm64)
    CROSS_COMPILE=aarch64-linux-gnu-
else ifeq ($(ARCH),aarch64)
    CROSS_COMPILE=aarch64-linux-gnu-
else ifeq ($(ARCH),riscv64)
    CROSS_COMPILE=riscv64-linux-gnu-
else
    $(error Unsupported architecture: $(ARCH))
endif

CC=$(CROSS_COMPILE)gcc
LD=$(CROSS_COMPILE)ld
OBJCOPY=$(CROSS_COMPILE)objcopy

CFLAGS=-nostdlib -nostartfiles -ffreestanding -O2 -Wall -Wextra

ifeq ($(ARCH),x86_64)
    CFLAGS += -D__x86_64__
else ifeq ($(ARCH),arm64)
    CFLAGS += -D__aarch64__
else ifeq ($(ARCH),aarch64)
    CFLAGS += -D__aarch64__
else ifeq ($(ARCH),riscv64)
    CFLAGS += -D__riscv -D__riscv_xlen=64
endif
LDFLAGS=-T linker.ld

SOURCES = $(wildcard boot/*.S kernel/*.c drivers/*.c)
OBJECTS = $(SOURCES:.c=.o)
OBJECTS := $(OBJECTS:.S=.o)

all: kernel8.img
	@mkdir -p build/$(ARCH)
	@cp kernel8.img build/$(ARCH)/kernel.img

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.S
	$(CC) $(CFLAGS) -c $< -o $@

kernel8.img: $(OBJECTS)
	$(LD) $(LDFLAGS) -o kernel.elf $(OBJECTS)
	$(OBJCOPY) -O binary kernel.elf $@

clean:
	rm -f $(OBJECTS) kernel.elf kernel8.img
	rm -rf build
