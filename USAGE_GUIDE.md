# SAGE OS Usage Guide

This guide provides detailed instructions on how to build, run, test, and deploy SAGE OS across multiple architectures.

## Table of Contents

- [Prerequisites](#prerequisites)
- [Supported Architectures](#supported-architectures)
- [Building SAGE OS](#building-sage-os)
  - [Building for x86_64](#building-for-x86_64)
  - [Building for ARM64/AArch64](#building-for-arm64aarch64)
  - [Building for RISC-V](#building-for-risc-v)
  - [Building for Raspberry Pi](#building-for-raspberry-pi)
- [Running SAGE OS](#running-sage-os)
  - [Running in QEMU](#running-in-qemu)
  - [Running on Real Hardware](#running-on-real-hardware)
- [Testing SAGE OS](#testing-sage-os)
  - [Automated Tests](#automated-tests)
  - [Manual Testing](#manual-testing)
- [Debugging SAGE OS](#debugging-sage-os)
  - [Using GDB](#using-gdb)
  - [Using QEMU Debug Features](#using-qemu-debug-features)
- [Deploying SAGE OS](#deploying-sage-os)
  - [Creating Bootable Media](#creating-bootable-media)
  - [Network Boot](#network-boot)
- [Contributing to SAGE OS](#contributing-to-sage-os)
  - [Code Style](#code-style)
  - [Pull Request Process](#pull-request-process)
- [Troubleshooting](#troubleshooting)

## Prerequisites

To build and run SAGE OS, you'll need the following tools and dependencies:

### Common Requirements

- **Git**: For version control
- **Make**: For build automation
- **QEMU**: For emulation (version 6.0 or higher recommended)
- **GCC**: For compilation
- **Binutils**: For binary utilities
- **Python 3**: For build scripts

### Architecture-Specific Requirements

#### For x86_64

```bash
sudo apt-get install gcc binutils-x86-64-linux-gnu qemu-system-x86
```

#### For ARM64/AArch64

```bash
sudo apt-get install gcc-aarch64-linux-gnu binutils-aarch64-linux-gnu qemu-system-aarch64
```

#### For RISC-V

```bash
sudo apt-get install gcc-riscv64-linux-gnu binutils-riscv64-linux-gnu qemu-system-riscv64
```

#### For Raspberry Pi

```bash
sudo apt-get install gcc-arm-linux-gnueabihf binutils-arm-linux-gnueabihf
```

## Supported Architectures

SAGE OS currently supports the following architectures:

- **x86_64**: 64-bit x86 architecture
- **ARM64/AArch64**: 64-bit ARM architecture
- **RISC-V (riscv64)**: 64-bit RISC-V architecture
- **Raspberry Pi**: Various Raspberry Pi models (ARM-based)

## Building SAGE OS

### Building for x86_64

To build SAGE OS for x86_64 architecture:

```bash
make ARCH=x86_64
```

The compiled kernel will be available at `build/x86_64/kernel.bin`.

### Building for ARM64/AArch64

To build SAGE OS for ARM64/AArch64 architecture:

```bash
make ARCH=arm64
```

or

```bash
make ARCH=aarch64
```

The compiled kernel will be available at `build/arm64/kernel.bin`.

### Building for RISC-V

To build SAGE OS for RISC-V architecture:

```bash
make ARCH=riscv64
```

The compiled kernel will be available at `build/riscv64/kernel.bin`.

### Building for Raspberry Pi

To build SAGE OS for Raspberry Pi:

```bash
make ARCH=raspi
```

The compiled kernel will be available at `build/raspi/kernel.img`.

### Building All Architectures

To build SAGE OS for all supported architectures:

```bash
make all-arch
```

### Cleaning Build Files

To clean build files for a specific architecture:

```bash
make clean ARCH=x86_64
```

To clean all build files:

```bash
make clean-all
```

## Running SAGE OS

### Running in QEMU

#### Running x86_64 Build

```bash
make run ARCH=x86_64
```

or manually:

```bash
qemu-system-x86_64 -kernel build/x86_64/kernel.bin -serial stdio -m 512M
```

#### Running ARM64/AArch64 Build

```bash
make run ARCH=arm64
```

or manually:

```bash
qemu-system-aarch64 -machine virt -cpu cortex-a57 -kernel build/arm64/kernel.bin -serial stdio -m 512M
```

#### Running RISC-V Build

```bash
make run ARCH=riscv64
```

or manually:

```bash
qemu-system-riscv64 -machine virt -kernel build/riscv64/kernel.bin -serial stdio -m 512M
```

### Running on Real Hardware

#### Running on x86_64 Hardware

1. Create a bootable USB drive:
   ```bash
   make bootable-usb ARCH=x86_64 DEVICE=/dev/sdX
   ```
   Replace `/dev/sdX` with your USB device path.

2. Boot from the USB drive on your target machine.

#### Running on Raspberry Pi

1. Copy the kernel image to an SD card with Raspberry Pi OS:
   ```bash
   cp build/raspi/kernel.img /path/to/sd/card/
   ```

2. Insert the SD card into your Raspberry Pi and power it on.

## Testing SAGE OS

### Automated Tests

SAGE OS includes automated tests to verify functionality. To run the tests:

```bash
make test
```

To run tests for a specific architecture:

```bash
make test ARCH=x86_64
```

### Manual Testing

For manual testing, you can use the QEMU emulator with the debug console:

```bash
make run-debug ARCH=x86_64
```

This will start QEMU with the kernel and enable the debug console, allowing you to interact with the system.

## Debugging SAGE OS

### Using GDB

To debug SAGE OS with GDB:

```bash
make debug ARCH=x86_64
```

This will start QEMU in debug mode and wait for a GDB connection. In another terminal, run:

```bash
gdb
(gdb) target remote localhost:1234
(gdb) file build/x86_64/kernel.elf
(gdb) break kernel_main
(gdb) continue
```

### Using QEMU Debug Features

QEMU provides various debug features that can be useful:

```bash
qemu-system-x86_64 -kernel build/x86_64/kernel.bin -serial stdio -d int,cpu_reset -D qemu.log
```

This will log interrupts and CPU resets to `qemu.log`.

## Deploying SAGE OS

### Creating Bootable Media

#### Creating a Bootable USB Drive

```bash
make bootable-usb ARCH=x86_64 DEVICE=/dev/sdX
```

Replace `/dev/sdX` with your USB device path.

#### Creating a Bootable ISO

```bash
make iso ARCH=x86_64
```

This will create an ISO file at `build/x86_64/sage-os.iso`.

### Network Boot

SAGE OS supports network booting via PXE. To set up a PXE server:

1. Copy the kernel to your TFTP server:
   ```bash
   cp build/x86_64/kernel.bin /var/lib/tftpboot/
   ```

2. Configure your DHCP server to point to the TFTP server and the kernel file.

## Contributing to SAGE OS

### Code Style

SAGE OS follows a specific code style:

- Use 4 spaces for indentation (no tabs)
- Maximum line length of 100 characters
- Use camelCase for function names and variables
- Use PascalCase for struct and enum names
- Add license headers to all source files

### Pull Request Process

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Run tests to ensure functionality
5. Submit a pull request

## Troubleshooting

### Common Build Issues

#### Missing Compiler

If you encounter errors about missing compilers, ensure you have installed the appropriate cross-compiler for your target architecture.

#### QEMU Not Found

If QEMU commands fail, make sure QEMU is installed for the appropriate architecture.

#### Build Fails with Syntax Errors

Ensure you're using a compatible version of GCC for the target architecture.

### Common Runtime Issues

#### Kernel Panic on Boot

This could be due to incompatible hardware or incorrect boot parameters. Try running in QEMU first to verify the kernel works.

#### No Serial Output

Check that your serial connection is properly configured. For QEMU, ensure you're using the `-serial stdio` option.

---

For additional help or to report issues, please open an issue on the GitHub repository.

© 2025 SAGE OS Project. All rights reserved.