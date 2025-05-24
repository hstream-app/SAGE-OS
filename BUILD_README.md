<!--
─────────────────────────────────────────────────────────────────────────────
SAGE OS — Copyright (c) 2025 Ashish Vasant Yesale (ashishyesale007@gmail.com)
SPDX-License-Identifier: BSD-3-Clause OR Proprietary
SAGE OS is dual-licensed under the BSD 3-Clause License and a Commercial License.

This file is part of the SAGE OS Project.
─────────────────────────────────────────────────────────────────────────────
-->
# 🏗️ SAGE OS Multi-Architecture Build System

A comprehensive, cross-platform build system for SAGE OS supporting multiple architectures, platforms, and image formats.

## 🚀 Quick Start

### macOS Users (Recommended)
```bash
# Interactive build menu
./build-macos.sh

# Or use the command line
./build.sh install-deps
./build.sh build aarch64 rpi5
```

### Linux Users
```bash
# Install dependencies
sudo apt-get install gcc-aarch64-linux-gnu qemu-system-arm
./build.sh build aarch64 rpi4
```

### Docker Users (Any Platform)
```bash
# Build with Docker (no local toolchain needed)
./docker-build.sh build aarch64 rpi5
./docker-build.sh build-all
```

## 📋 What You Can Build

| Target | Architecture | Platform | Image Format | Use Case |
|--------|--------------|----------|--------------|----------|
| 🥧 **Raspberry Pi 4** | ARM64 | rpi4 | kernel, sdcard | Direct Pi deployment |
| 🥧 **Raspberry Pi 5** | ARM64 | rpi5 | kernel, sdcard | Latest Pi with AI HAT+ |
| 💻 **x86_64 Desktop** | x86_64 | x86_64 | kernel, iso | PC/laptop deployment |
| 🔬 **RISC-V** | riscv64 | generic | kernel | Experimental/research |
| 📱 **ARM32** | arm | rpi3 | kernel, sdcard | Older Pi models |

## 🛠️ Build Tools

### 1. Interactive macOS Builder (`build-macos.sh`)
Perfect for Mac users who want a simple, menu-driven interface:

```bash
./build-macos.sh
```

Features:
- 🍎 macOS-optimized with Homebrew integration
- 📋 Interactive menu system
- 🔧 Automatic dependency installation
- 📊 Build status and file size reporting
- 💿 SD card and ISO creation helpers

### 2. Command Line Builder (`build.sh`)
Cross-platform command-line interface:

```bash
# Install dependencies
./build.sh install-deps

# Build for specific target
./build.sh build aarch64 rpi5

# Build all architectures
./build.sh build-all

# Build all image formats
./build.sh build-formats aarch64

# Test with QEMU
./build.sh test aarch64 rpi4

# Show status
./build.sh status
```

### 3. Docker Builder (`docker-build.sh`)
Containerized builds for consistency:

```bash
# Build with Docker (no local setup needed)
./docker-build.sh build aarch64 rpi5

# Build all architectures
./docker-build.sh build-all

# Create multi-arch Docker images
./docker-build.sh multiarch
```

### 4. Direct Makefile (`Makefile.multi-arch`)
Low-level control for advanced users:

```bash
# Build kernel
make -f Makefile.multi-arch kernel ARCH=aarch64 PLATFORM=rpi5

# Build ISO
make -f Makefile.multi-arch iso ARCH=x86_64

# Build SD card image
make -f Makefile.multi-arch sdcard ARCH=aarch64 PLATFORM=rpi4

# Run in QEMU
make -f Makefile.multi-arch qemu ARCH=aarch64 PLATFORM=rpi4
```

## 🎯 Common Use Cases

### Building for Raspberry Pi 5 with AI Features
```bash
# Method 1: Interactive (macOS)
./build-macos.sh
# Choose option 3 (Raspberry Pi 5)

# Method 2: Command line
ENABLE_AI=ON ./build.sh build aarch64 rpi5

# Method 3: Docker
ENABLE_AI=ON ./docker-build.sh build aarch64 rpi5

# Method 4: Direct make
make -f Makefile.multi-arch kernel ARCH=aarch64 PLATFORM=rpi5 ENABLE_AI=ON
```

### Creating Bootable SD Card for Raspberry Pi
```bash
# Build and create SD card image
./build.sh build aarch64 rpi4
make -f Makefile.multi-arch sdcard ARCH=aarch64 PLATFORM=rpi4

# Flash to SD card (macOS)
diskutil list
diskutil unmountDisk /dev/diskN
sudo dd if=dist/aarch64/SAGE-OS-0.1.0-aarch64-rpi4-sdcard.img of=/dev/rdiskN bs=1m
```

### Building x86_64 ISO for PC
```bash
# Build and create ISO
./build.sh build x86_64 x86_64
make -f Makefile.multi-arch iso ARCH=x86_64

# Create bootable USB (macOS)
sudo dd if=dist/x86_64/SAGE-OS-0.1.0-x86_64-x86_64.iso of=/dev/rdiskN bs=1m
```

### Testing with QEMU
```bash
# Quick test
./build.sh test aarch64 rpi4

# Debug with GDB
make -f Makefile.multi-arch debug ARCH=aarch64 PLATFORM=rpi4
# In another terminal:
aarch64-linux-gnu-gdb build/aarch64/kernel.elf -ex 'target remote localhost:1234'
```

## 🔧 Configuration Options

### Environment Variables
```bash
export ARCH=aarch64              # Target architecture
export PLATFORM=rpi5             # Target platform
export ENABLE_AI=ON              # Enable AI subsystem
export ENABLE_CRYPTO=ON          # Enable crypto support
export ENABLE_DEBUG=ON           # Debug build
export MEMORY_SIZE=2048          # Memory size in MB
```

### Feature Flags
- `ENABLE_AI=ON` - AI subsystem and HAT+ support
- `ENABLE_CRYPTO=ON` - Cryptographic functions
- `ENABLE_DEBUG=ON` - Debug symbols and logging
- `MEMORY_SIZE=1024` - Available memory in MB

## 📁 Output Files

After building, you'll find:

```
build/
├── aarch64/
│   ├── kernel.img      # Raw kernel binary
│   ├── kernel.elf      # ELF with debug symbols
│   ├── kernel.map      # Memory map
│   └── kernel.dump     # Disassembly
└── x86_64/
    └── ...

dist/
├── aarch64/
│   ├── SAGE-OS-0.1.0-aarch64-rpi5.img         # Kernel image
│   ├── SAGE-OS-0.1.0-aarch64-rpi5.iso         # ISO image
│   └── SAGE-OS-0.1.0-aarch64-rpi5-sdcard.img  # SD card image
└── x86_64/
    └── ...
```

## 🐳 Docker Images

The build system can create Docker images:

```bash
# Build Docker image
./docker-build.sh build aarch64 rpi5

# Create multi-arch images
./docker-build.sh multiarch

# Run SAGE OS in Docker
docker run --rm -it sage-os:latest
```

## 🧪 Testing and Validation

### Automated Testing
```bash
# Test specific build
./build.sh test aarch64 rpi4

# Test all builds
for arch in aarch64 x86_64 riscv64; do
    ./build.sh test $arch generic
done
```

### Manual Testing
1. **QEMU Emulation**: Test without hardware
2. **Hardware Deployment**: Flash to real devices
3. **Docker Containers**: Test in isolated environments

## 🔍 Troubleshooting

### Common Issues

#### Missing Cross-Compiler
```bash
# macOS
./build.sh install-deps

# Linux
sudo apt-get install gcc-aarch64-linux-gnu
```

#### QEMU Not Found
```bash
# macOS
brew install qemu

# Linux
sudo apt-get install qemu-system-arm
```

#### Permission Denied (Docker)
```bash
sudo usermod -aG docker $USER
# Log out and back in
```

### Debug Build Issues
```bash
# Verbose build
make -f Makefile.multi-arch kernel ARCH=aarch64 V=1

# Check configuration
make -f Makefile.multi-arch info ARCH=aarch64

# Clean rebuild
./build.sh clean
./build.sh build aarch64 rpi4
```

## 📊 Performance Tips

### Parallel Builds
```bash
# Use all CPU cores
make -f Makefile.multi-arch -j$(nproc) kernel ARCH=aarch64
```

### Build Caching
```bash
# Enable ccache (if available)
export CC="ccache aarch64-linux-gnu-gcc"
```

### Docker Optimization
```bash
# Use BuildKit
export DOCKER_BUILDKIT=1
./docker-build.sh build aarch64 rpi5
```

## 🚀 CI/CD Integration

The project includes GitHub Actions workflows:

- **Multi-Architecture Builds**: Automatic builds for all supported targets
- **Artifact Generation**: Downloadable kernel images, ISOs, and SD card images
- **Docker Publishing**: Multi-arch Docker images
- **Automated Releases**: Tagged releases with all build artifacts

## 🤝 Contributing

When contributing to the build system:

1. Test on multiple platforms (macOS, Linux)
2. Ensure Docker builds work
3. Update documentation
4. Add tests for new features
5. Maintain backward compatibility

## 📚 Documentation

- **[Complete Build System Guide](docs/BUILD_SYSTEM.md)** - Detailed documentation
- **[Architecture Support](docs/ARCHITECTURES.md)** - Supported architectures
- **[Platform Guide](docs/PLATFORMS.md)** - Platform-specific information
- **[Docker Guide](docs/DOCKER.md)** - Container-based builds

## 🎯 Examples

### Raspberry Pi 5 with AI HAT+
```bash
# Interactive (macOS)
./build-macos.sh
# Choose: 3) Build for Raspberry Pi 5

# Command line with AI enabled
ENABLE_AI=ON ENABLE_AI_HAT_PLUS=ON ./build.sh build aarch64 rpi5

# Create SD card image
make -f Makefile.multi-arch sdcard ARCH=aarch64 PLATFORM=rpi5 ENABLE_AI=ON
```

### x86_64 Development System
```bash
# Build bootable ISO
./build.sh build x86_64 x86_64
make -f Makefile.multi-arch iso ARCH=x86_64

# Test in QEMU
./build.sh test x86_64 x86_64
```

### RISC-V Research Build
```bash
# Build for RISC-V
./build.sh build riscv64 generic

# Test with QEMU
make -f Makefile.multi-arch qemu ARCH=riscv64 PLATFORM=generic
```

### Complete Multi-Arch Release
```bash
# Build everything
./build.sh build-all

# Create all image formats
for arch in aarch64 x86_64; do
    ./build.sh build-formats $arch
done

# Check status
./build.sh status
```

---

## 🏆 Features Summary

✅ **Multi-Architecture**: ARM64, x86_64, RISC-V, ARM32  
✅ **Multi-Platform**: Raspberry Pi 3/4/5, x86_64, generic  
✅ **Multi-Format**: Kernel, ISO, SD card, Docker images  
✅ **Cross-Platform**: macOS, Linux, Docker  
✅ **Interactive**: Menu-driven interface for macOS  
✅ **Automated**: CI/CD with GitHub Actions  
✅ **Tested**: QEMU emulation and hardware validation  
✅ **Documented**: Comprehensive guides and examples  

**Ready to build SAGE OS for any target platform! 🚀**