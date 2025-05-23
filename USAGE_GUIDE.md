# SAGE OS Usage Guide

This guide provides detailed instructions on how to build, run, test, and deploy SAGE OS across multiple architectures.

## Table of Contents

1. [Prerequisites](#prerequisites)
2. [Building SAGE OS](#building-sage-os)
3. [Running SAGE OS](#running-sage-os)
4. [Testing SAGE OS](#testing-sage-os)
5. [Deploying SAGE OS](#deploying-sage-os)
6. [Troubleshooting](#troubleshooting)
7. [Development Workflow](#development-workflow)

## Prerequisites

### Required Tools

To work with SAGE OS, you need the following tools:

#### Cross-Compilers

- **x86_64**: `x86_64-linux-gnu-gcc`
- **ARM64/AArch64**: `aarch64-linux-gnu-gcc`
- **RISC-V 64-bit**: `riscv64-linux-gnu-gcc`

#### Emulation Tools

- **QEMU** for emulating different architectures:
  - `qemu-system-x86_64`
  - `qemu-system-aarch64`
  - `qemu-system-riscv64`

#### Development Tools

- **Git** for version control
- **Make** for build automation
- **Python 3** for utility scripts

### Installation Instructions

#### Debian/Ubuntu

```bash
# Install cross-compilers
sudo apt-get update
sudo apt-get install -y gcc-x86-64-linux-gnu gcc-aarch64-linux-gnu gcc-riscv64-linux-gnu

# Install QEMU
sudo apt-get install -y qemu-system-x86 qemu-system-arm qemu-system-misc

# Install development tools
sudo apt-get install -y git make python3 python3-pip
```

#### macOS

```bash
# Install Homebrew if not already installed
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install QEMU
brew install qemu

# Install cross-compilers via Homebrew
brew tap messense/macos-cross-toolchains
brew install x86_64-unknown-linux-gnu aarch64-unknown-linux-gnu riscv64-unknown-linux-gnu

# Install development tools
brew install git make python3
```

## Building SAGE OS

### Getting the Source Code

```bash
# Clone the repository
git clone https://github.com/AshishYesale7/SAGE-OS.git
cd SAGE-OS
```

### Building for Different Architectures

SAGE OS supports multiple architectures. Use the `ARCH` parameter to specify the target architecture.

#### Clean Previous Build Artifacts

```bash
make clean
```

#### Build for x86_64

```bash
make ARCH=x86_64
```

#### Build for ARM64/AArch64

```bash
make ARCH=aarch64
```

#### Build for RISC-V 64-bit

```bash
make ARCH=riscv64
```

### Build Artifacts

After a successful build, you'll find the following files:

- **Object files**: `.o` files for each source file
- **ELF executable**: `kernel.elf`
- **Raw binary image**: `kernel8.img` (for ARM64/AArch64)

## Running SAGE OS

### Using QEMU Emulation

SAGE OS can be run in QEMU for testing and development.

#### Running on x86_64

```bash
qemu-system-x86_64 -kernel kernel.elf -m 256 -serial stdio
```

#### Running on ARM64/AArch64

```bash
qemu-system-aarch64 -machine virt -cpu cortex-a53 -kernel kernel8.img -m 256 -serial stdio
```

#### Running on RISC-V 64-bit

```bash
qemu-system-riscv64 -machine virt -kernel kernel.elf -m 256 -serial stdio
```

### Using Convenience Scripts

The repository includes scripts to simplify running SAGE OS in QEMU:

```bash
# Run on x86_64
./scripts/test_emulated.sh x86_64

# Run on ARM64/AArch64
./scripts/test_emulated.sh aarch64

# Run on RISC-V 64-bit
./scripts/test_emulated.sh riscv64
```

### Running on Real Hardware

#### Raspberry Pi (ARM64)

1. Build SAGE OS for ARM64:
   ```bash
   make ARCH=aarch64
   ```

2. Copy the kernel image to your SD card:
   ```bash
   cp kernel8.img /path/to/sd/card/boot/
   ```

3. Ensure your `config.txt` on the SD card contains:
   ```
   arm_64bit=1
   kernel=kernel8.img
   ```

4. Insert the SD card into your Raspberry Pi and power it on.

## Testing SAGE OS

### Automated Tests

SAGE OS includes automated tests to verify functionality:

```bash
# Run all tests
make test

# Run architecture-specific tests
make test ARCH=x86_64
make test ARCH=aarch64
make test ARCH=riscv64
```

### Manual Testing

For manual testing, you can use QEMU with debugging enabled:

```bash
# Debug on x86_64
qemu-system-x86_64 -kernel kernel.elf -m 256 -serial stdio -s -S
```

Then connect with GDB:

```bash
gdb kernel.elf -ex "target remote localhost:1234"
```

### Continuous Integration

SAGE OS uses GitHub Actions for continuous integration. The workflows check:

1. **Build Test**: Verifies that SAGE OS builds successfully for all supported architectures
2. **License Headers**: Ensures all source files have proper license headers
3. **Code Quality**: Checks code style and quality
4. **Project Structure**: Validates the project structure

## Deploying SAGE OS

### Creating Release Images

To create release images for distribution:

```bash
# Create release images for all architectures
make release

# Create release image for a specific architecture
make release ARCH=aarch64
```

This creates compressed images in the `release/` directory.

### Deploying to Raspberry Pi

1. Download the latest release image for ARM64/AArch64
2. Extract the image:
   ```bash
   unzip sage-os-aarch64.zip
   ```
3. Write the image to your SD card:
   ```bash
   dd if=sage-os-aarch64.img of=/dev/sdX bs=4M status=progress
   ```
   Replace `/dev/sdX` with your SD card device.
4. Insert the SD card into your Raspberry Pi and power it on.

## Troubleshooting

### Common Build Issues

#### Missing Compiler

Error: `aarch64-linux-gnu-gcc: command not found`

Solution: Install the required cross-compiler:
```bash
sudo apt-get install gcc-aarch64-linux-gnu
```

#### Linker Errors

Error: `undefined reference to...`

Solution: Check that all required object files are included in the link step. Verify that you're using the correct architecture-specific code.

### Runtime Issues

#### Kernel Doesn't Boot

1. Verify that the boot code for your architecture is correctly implemented
2. Check that you're using the correct QEMU parameters
3. Ensure the kernel image is properly formatted for your target architecture

#### QEMU Crashes

1. Update QEMU to the latest version
2. Increase the memory allocation (`-m` parameter)
3. Check for architecture-specific issues in your code

## Development Workflow

### Adding New Features

1. Create a new branch:
   ```bash
   git checkout -b feature/my-new-feature
   ```

2. Implement your changes, ensuring:
   - All source files have proper license headers
   - Code follows the project's style guidelines
   - Architecture-specific code is properly isolated

3. Build and test your changes:
   ```bash
   make clean
   make ARCH=x86_64
   make ARCH=aarch64
   make ARCH=riscv64
   ```

4. Commit your changes:
   ```bash
   git add .
   git commit -m "Add my new feature"
   ```

5. Push your changes and create a pull request:
   ```bash
   git push origin feature/my-new-feature
   ```

### License Compliance

All source files must include the BSD 3-Clause License header. You can check for compliance using:

```bash
./license-checker.py
```

If you need to add license headers to new files:

```bash
./add_license_headers.py
```

---

## Additional Resources

- [SAGE OS GitHub Repository](https://github.com/AshishYesale7/SAGE-OS)
- [BSD 3-Clause License](https://opensource.org/licenses/BSD-3-Clause)
- [QEMU Documentation](https://www.qemu.org/docs/master/)
- [Raspberry Pi Documentation](https://www.raspberrypi.org/documentation/)