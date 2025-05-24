---
layout: default
title: Build System
nav_order: 4
has_children: true
---

# SAGE OS Build System
{: .no_toc }

Comprehensive build system documentation for multi-architecture support.
{: .fs-6 .fw-300 }

## Table of Contents
{: .no_toc .text-delta }

1. TOC
{:toc}

---

## Build System Overview

SAGE OS features a sophisticated multi-architecture build system supporting ARM64, ARM32, x86_64, and RISC-V architectures across multiple platforms.

### Build Architecture

```mermaid
graph TB
    subgraph "Build System Architecture"
        A[Build Scripts] --> B[Makefile System]
        B --> C[Cross Compilers]
        C --> D[Architecture Targets]
        
        A --> E[build-all-architectures.sh]
        A --> F[build-all-macos.sh]
        A --> G[build-macos.sh]
        A --> H[build.sh]
        
        B --> I[Makefile]
        B --> J[Makefile.multi-arch]
        
        C --> K[GCC Toolchains]
        C --> L[Clang Toolchains]
        
        D --> M[ARM64 Targets]
        D --> N[ARM32 Targets]
        D --> O[x86_64 Targets]
        D --> P[RISC-V Targets]
    end
```

## Supported Architectures

### Architecture Matrix

| Architecture | Platforms | Compiler | Status | Notes |
|-------------|-----------|----------|---------|-------|
| **aarch64** | rpi4, rpi5, generic | gcc-aarch64-linux-gnu | ✅ Full Support | Primary ARM64 target |
| **arm** | rpi4, rpi5, generic | gcc-arm-linux-gnueabihf | ✅ Full Support | 32-bit ARM support |
| **x86_64** | generic | gcc/clang | ✅ Full Support | Standard x86_64 |
| **riscv64** | generic | gcc-riscv64-linux-gnu | ✅ Linux Only | Excluded on macOS |

### Platform-Specific Features

```mermaid
graph LR
    subgraph "Platform Features"
        A[Raspberry Pi 4] --> B[BCM2711 SoC]
        A --> C[ARM Cortex-A72]
        A --> D[VideoCore VI GPU]
        
        E[Raspberry Pi 5] --> F[BCM2712 SoC]
        E --> G[ARM Cortex-A76]
        E --> H[VideoCore VII GPU]
        
        I[Generic ARM64] --> J[Standard ARM64]
        I --> K[UEFI Boot]
        I --> L[Generic Drivers]
        
        M[x86_64 Generic] --> N[Standard PC]
        M --> O[BIOS/UEFI Boot]
        M --> P[Legacy Support]
    end
```

## Build Scripts

### 1. `build-all-architectures.sh` - Complete Multi-Architecture Build

**Purpose**: Build all supported architectures on Linux/WSL

**Features**:
- Builds 8 architecture/platform combinations
- Generates both .img and .elf files
- Comprehensive error handling
- Progress tracking and reporting

**Usage**:
```bash
chmod +x build-all-architectures.sh
./build-all-architectures.sh
```

**Build Flow**:
```mermaid
sequenceDiagram
    participant User as User
    participant Script as Build Script
    participant Make as Makefile System
    participant Compiler as Cross Compiler
    participant Output as Build Output

    User->>Script: Execute build script
    Script->>Script: Check dependencies
    Script->>Make: Build aarch64-rpi4
    Make->>Compiler: Compile sources
    Compiler-->>Make: Object files
    Make->>Output: Generate kernel.img
    Make->>Output: Generate kernel.elf
    Make-->>Script: Build complete
    Script->>Script: Repeat for all targets
    Script-->>User: All builds complete
```

### 2. `build-all-macos.sh` - macOS Compatible Build

**Purpose**: Build supported architectures on macOS (excludes RISC-V)

**macOS Compatibility Features**:
- Bash 3.2+ compatibility
- Homebrew dependency management
- Automatic toolchain detection
- RISC-V exclusion for compatibility

**Differences from Linux Build**:
```mermaid
graph TB
    subgraph "Linux Build"
        A1[All 8 Architectures]
        A2[RISC-V Included]
        A3[APT Package Manager]
        A4[Bash 4.0+ Features]
    end
    
    subgraph "macOS Build"
        B1[7 Architectures]
        B2[RISC-V Excluded]
        B3[Homebrew Package Manager]
        B4[Bash 3.2 Compatible]
    end
    
    A1 --> B1
    A2 --> B2
    A3 --> B3
    A4 --> B4
```

### 3. `build-macos.sh` - Interactive macOS Build System

**Purpose**: Interactive menu-driven build system for macOS

**Menu Options**:
1. 🔧 Install build dependencies
2. 🏗️ Build for Raspberry Pi 4 (ARM64)
3. 🏗️ Build for Raspberry Pi 5 (ARM64)
4. 🏗️ Build for x86_64
5. 🏗️ Build for all architectures
6. 💿 Create SD card image for Raspberry Pi
7. 💿 Create ISO image for x86_64
8. 🧪 Test build with QEMU
9. 🐳 Build with Docker
10. 📊 Show build status
11. 🧹 Clean build files
12. ❓ Show detailed help

**Interactive Flow**:
```mermaid
graph TD
    A[Start build-macos.sh] --> B[Display Menu]
    B --> C{User Selection}
    
    C -->|1| D[Install Dependencies]
    C -->|2-4| E[Single Architecture Build]
    C -->|5| F[All Architectures Build]
    C -->|6-7| G[Create Images]
    C -->|8| H[QEMU Testing]
    C -->|9| I[Docker Build]
    C -->|10| J[Show Status]
    C -->|11| K[Clean Files]
    C -->|12| L[Show Help]
    C -->|0| M[Exit]
    
    D --> B
    E --> B
    F --> B
    G --> B
    H --> B
    I --> B
    J --> B
    K --> B
    L --> B
```

## Makefile System

### Primary Makefile Structure

```mermaid
graph TB
    subgraph "Makefile Hierarchy"
        A[Makefile] --> B[Default Targets]
        A --> C[Architecture Detection]
        A --> D[Compiler Configuration]
        
        E[Makefile.multi-arch] --> F[Multi-Arch Targets]
        E --> G[Cross-Compilation Rules]
        E --> H[Platform-Specific Settings]
        
        B --> I[kernel]
        B --> J[clean]
        B --> K[install]
        
        F --> L[aarch64 targets]
        F --> M[arm targets]
        F --> N[x86_64 targets]
        F --> O[riscv64 targets]
    end
```

### Build Targets

#### Core Targets

```makefile
# Primary kernel target
kernel: $(KERNEL_IMG) $(KERNEL_ELF)

# Architecture-specific targets
kernel-aarch64-rpi4: ARCH=aarch64 PLATFORM=rpi4
kernel-aarch64-rpi5: ARCH=aarch64 PLATFORM=rpi5
kernel-arm-rpi4: ARCH=arm PLATFORM=rpi4
kernel-x86_64-generic: ARCH=x86_64 PLATFORM=generic

# Clean targets
clean: clean-build clean-output
clean-all: clean clean-deps
```

#### Compilation Rules

```makefile
# Object file compilation
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Assembly file compilation
$(BUILD_DIR)/%.o: %.S
	@mkdir -p $(dir $@)
	$(CC) $(ASFLAGS) -c $< -o $@

# Kernel image generation
$(KERNEL_IMG): $(OBJECTS)
	$(LD) $(LDFLAGS) -o $(KERNEL_ELF) $(OBJECTS)
	$(OBJCOPY) -O binary $(KERNEL_ELF) $(KERNEL_IMG)
```

## Cross-Compilation Setup

### Toolchain Requirements

#### Linux/WSL Dependencies

```bash
# Install cross-compilation toolchains
sudo apt-get update
sudo apt-get install -y \
    gcc-aarch64-linux-gnu \
    gcc-arm-linux-gnueabihf \
    gcc-riscv64-linux-gnu \
    qemu-system-arm \
    qemu-system-aarch64 \
    qemu-system-x86 \
    qemu-system-riscv64
```

#### macOS Dependencies

```bash
# Install via Homebrew
brew install \
    aarch64-elf-gcc \
    arm-none-eabi-gcc \
    qemu
```

### Compiler Configuration

```mermaid
graph LR
    subgraph "Compiler Selection"
        A[Architecture] --> B{Target Platform}
        
        B -->|aarch64| C[gcc-aarch64-linux-gnu]
        B -->|arm| D[gcc-arm-linux-gnueabihf]
        B -->|x86_64| E[gcc/clang]
        B -->|riscv64| F[gcc-riscv64-linux-gnu]
        
        C --> G[ARM64 Object Files]
        D --> H[ARM32 Object Files]
        E --> I[x86_64 Object Files]
        F --> J[RISC-V Object Files]
    end
```

## Build Process Flow

### Complete Build Sequence

```mermaid
sequenceDiagram
    participant User as User
    participant Script as Build Script
    participant Deps as Dependency Check
    participant Make as Makefile
    participant Compiler as Cross Compiler
    participant Linker as Linker
    participant Output as Output Generation

    User->>Script: Start build
    Script->>Deps: Check dependencies
    Deps-->>Script: Dependencies OK
    
    loop For each architecture
        Script->>Make: make kernel ARCH=target
        Make->>Compiler: Compile source files
        Compiler-->>Make: Object files
        Make->>Linker: Link objects
        Linker-->>Make: ELF executable
        Make->>Output: Generate kernel image
        Output-->>Make: IMG file created
        Make-->>Script: Build complete
    end
    
    Script-->>User: All builds successful
```

### File Generation Process

```mermaid
graph TD
    A[Source Files] --> B[Preprocessing]
    B --> C[Compilation]
    C --> D[Object Files]
    D --> E[Linking]
    E --> F[ELF Executable]
    F --> G[Binary Extraction]
    G --> H[Kernel Image]
    
    F --> I[Debug Symbols]
    H --> J[Deployment Ready]
    I --> K[Development Tools]
```

## Build Output Structure

### Generated Files

```
build-output/
├── kernel-aarch64-generic.elf    # ARM64 generic ELF
├── kernel-aarch64-generic.img    # ARM64 generic image
├── kernel-aarch64-rpi4.elf       # ARM64 RPi4 ELF
├── kernel-aarch64-rpi4.img       # ARM64 RPi4 image
├── kernel-aarch64-rpi5.elf       # ARM64 RPi5 ELF
├── kernel-aarch64-rpi5.img       # ARM64 RPi5 image
├── kernel-arm-generic.elf        # ARM32 generic ELF
├── kernel-arm-generic.img        # ARM32 generic image
├── kernel-arm-rpi4.elf           # ARM32 RPi4 ELF
├── kernel-arm-rpi4.img           # ARM32 RPi4 image
├── kernel-arm-rpi5.elf           # ARM32 RPi5 ELF
├── kernel-arm-rpi5.img           # ARM32 RPi5 image
├── kernel-riscv64-generic.elf    # RISC-V ELF
├── kernel-riscv64-generic.img    # RISC-V image
├── kernel-x86_64-generic.elf     # x86_64 ELF
└── kernel-x86_64-generic.img     # x86_64 image
```

### File Size Analysis

```mermaid
pie title Build Output Sizes
    "ARM64 Files (6)" : 40
    "ARM32 Files (6)" : 25
    "x86_64 Files (2)" : 20
    "RISC-V Files (2)" : 15
```

## Testing and Validation

### QEMU Testing

```bash
# Test ARM64 build
./run_qemu.sh aarch64 rpi4

# Test x86_64 build
./run_qemu.sh x86_64 generic

# Test all builds
./test-all-builds.sh
```

### Build Validation

```mermaid
graph TD
    A[Build Complete] --> B[File Validation]
    B --> C[Size Check]
    C --> D[Format Verification]
    D --> E[Symbol Analysis]
    E --> F[QEMU Boot Test]
    F --> G{All Tests Pass?}
    G -->|Yes| H[Build Success]
    G -->|No| I[Build Failed]
```

## Troubleshooting

### Common Build Issues

#### 1. Missing Cross-Compilers

**Error**: `Cross-compiler not found`

**Solution**:
```bash
# Linux/WSL
sudo apt-get install gcc-aarch64-linux-gnu

# macOS
brew install aarch64-elf-gcc
```

#### 2. macOS Bash Compatibility

**Error**: `Syntax error: bad array subscript`

**Solution**: Use `build-all-macos.sh` instead of `build-all-architectures.sh`

#### 3. RISC-V Build Failure on macOS

**Error**: `riscv64-linux-gnu-gcc not found`

**Solution**: RISC-V is excluded on macOS by design. Use Linux/WSL for RISC-V builds.

### Build System Diagnostics

```mermaid
graph TD
    A[Build Failure] --> B{Error Type}
    
    B -->|Compiler Missing| C[Install Toolchain]
    B -->|Source Error| D[Check Code Syntax]
    B -->|Linker Error| E[Check Dependencies]
    B -->|Permission Error| F[Check File Permissions]
    
    C --> G[Retry Build]
    D --> H[Fix Source Code]
    E --> I[Update Makefile]
    F --> J[Fix Permissions]
    
    G --> K{Success?}
    H --> K
    I --> K
    J --> K
    
    K -->|Yes| L[Build Complete]
    K -->|No| M[Escalate Issue]
```

## Performance Optimization

### Build Time Analysis

| Architecture | Compile Time | Link Time | Total Time |
|-------------|--------------|-----------|------------|
| **aarch64** | 15s | 3s | 18s |
| **arm** | 12s | 2s | 14s |
| **x86_64** | 10s | 2s | 12s |
| **riscv64** | 18s | 4s | 22s |

### Parallel Build Support

```makefile
# Enable parallel compilation
MAKEFLAGS += -j$(shell nproc)

# Parallel architecture builds
build-parallel:
	$(MAKE) -j4 all-architectures
```

## Next Steps

- [Deployment Guide](../deployment/) - Production deployment
- [Testing Framework](../testing/) - Comprehensive testing
- [Development Setup](development.md) - Development environment
- [CI/CD Integration](ci-cd.md) - Continuous integration