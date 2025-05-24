# macOS Build Guide for SAGE OS

This guide provides specific instructions for building SAGE OS on macOS systems.

## Quick Start

### Option 1: Use the macOS Build Script (Recommended)
```bash
./build-macos.sh
# Select option 5 for "Build for all architectures"
```

### Option 2: Use the Direct Build Script
```bash
./build-all-architectures-macos.sh
```

### Option 3: Use the Universal Build Command
```bash
./build.sh build-all
```

## macOS-Specific Issues and Solutions

### 1. Bash Compatibility Issue

**Problem**: The original `build-all-architectures.sh` script uses bash associative arrays (`declare -A`) which are not supported in macOS's default bash 3.2.

**Error Message**:
```
./build-all-architectures.sh: line 22: declare: -A: invalid option
declare: usage: declare [-afFirtx] [-p] [name[=value] ...]
```

**Solution**: Use `build-all-architectures-macos.sh` which is compatible with older bash versions.

### 2. RISC-V Toolchain Missing

**Problem**: RISC-V cross-compiler is not installed by default on macOS.

**Error Message**:
```
Error: Cross-compiler /opt/homebrew/bin/riscv64-linux-gnu-gcc not found.
Run 'make install-deps' to install required toolchains.
```

**Solution**: The macOS-specific script automatically detects missing RISC-V toolchain and skips those builds gracefully.

To install RISC-V support:
```bash
brew install riscv64-elf-gcc
```

### 3. Architecture-Specific Issues

**ARM64 WFE Instruction on x86_64**: 
- The macOS script builds each architecture with proper cross-compilation
- Platform-specific code is properly isolated

## Build Output

All builds save their output to the `build-output/` directory:

```
build-output/
├── kernel-aarch64-generic.img
├── kernel-aarch64-generic.elf
├── kernel-aarch64-rpi4.img
├── kernel-aarch64-rpi4.elf
├── kernel-aarch64-rpi5.img
├── kernel-aarch64-rpi5.elf
├── kernel-arm-generic.img
├── kernel-arm-generic.elf
├── kernel-arm-rpi4.img
├── kernel-arm-rpi4.elf
├── kernel-arm-rpi5.img
├── kernel-arm-rpi5.elf
├── kernel-x86_64-generic.img
├── kernel-x86_64-generic.elf
├── kernel-riscv64-generic.img    # Only if RISC-V toolchain is installed
└── kernel-riscv64-generic.elf    # Only if RISC-V toolchain is installed
```

## Dependencies

### Required Dependencies
- Xcode Command Line Tools
- Homebrew
- Cross-compilation toolchains:
  - `aarch64-linux-gnu-gcc` (ARM64)
  - `arm-linux-gnueabihf-gcc` (ARM32)
  - `gcc` (x86_64)

### Optional Dependencies
- `riscv64-linux-gnu-gcc` (RISC-V) - will be skipped if not available

### Installing Dependencies
```bash
# Install via the build script
./build.sh install-deps

# Or manually via Homebrew
brew install aarch64-linux-gnu-gcc
brew install arm-linux-gnueabihf-gcc
brew install riscv64-elf-gcc  # Optional
```

## Troubleshooting

### Build Fails with "Invalid Architecture"
This usually indicates the bash compatibility issue. Use the macOS-specific script:
```bash
./build-all-architectures-macos.sh
```

### RISC-V Builds Fail
This is expected if the RISC-V toolchain is not installed. The macOS script will skip these builds automatically and report:
```
⚠️  RISC-V toolchain not found, skipping RISC-V builds
   To install: brew install riscv64-elf-gcc
```

### Permission Denied Errors
Make sure the scripts are executable:
```bash
chmod +x build-all-architectures-macos.sh
chmod +x build-macos.sh
chmod +x build.sh
```

## Testing Builds

After building, you can test the kernels using QEMU:
```bash
./test-all-builds.sh
```

Or test individual architectures:
```bash
# Test ARM64
qemu-system-aarch64 -M virt -cpu cortex-a57 -kernel build-output/kernel-aarch64-generic.elf -nographic

# Test x86_64
qemu-system-x86_64 -kernel build-output/kernel-x86_64-generic.elf -nographic

# Test ARM32
qemu-system-arm -M virt -cpu cortex-a15 -kernel build-output/kernel-arm-generic.elf -nographic
```

## Performance Comparison

You can benchmark the builds using:
```bash
./benchmark-builds.sh
```

This will show build times and kernel sizes for each architecture.