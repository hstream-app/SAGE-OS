
# SAGE OS 🌱🧠
**Self-Aware General Environment**  
An experimental, bare-metal operating system designed from scratch to learn, adapt, and evolve — starting on Raspberry Pi.

![License](https://img.shields.io/badge/License-BSD%203--Clause-blue.svg)
---
 
  ## 🔭 Vision 

**SAGE OS** is not just another operating system. It is a living, evolving system designed to:

- Think autonomously
- Learn from usage patterns
- Optimize itself through artificial intelligence
- Rebuild and evolve its components as needed

This is an experiment at the intersection of **kernel engineering**, **embedded systems**, and **machine intelligence** — pushing the boundaries of what an operating system can become.

<img src="https://github.com/user-attachments/assets/a5aeda94-147c-483f-8378-cca4ba023a13" width="600" height="500">


 
<details>
  <summary>🎯 Objectives</summary>

- Build a fully bootable, bare-metal operating system from scratch
- Integrate basic AI/ML models to inform system-level decisions
- Enable self-monitoring and self-optimization over time
- Keep the codebase portable across architectures (starting with ARM/RPi)
  
<img src="https://github.com/user-attachments/assets/a726baa8-dca4-4097-ab31-54b8cd819287" width="600" height="700">

  This image illustrates the **"Self-Evolving, AI-Driven OS Lifecycle"** — a conceptual flow for how your SAGE OS can autonomously detect hardware, build itself, and evolve across platforms.

---

### 🔍 Step-by-Step Breakdown:

1. **AI Detects Architecture and Hardware**

   * On boot or install, SAGE OS's embedded AI analyzes the CPU type, memory, buses, I/O interfaces, and connected devices.

2. **Generate Platform-Specific Kernel and Drivers**

   * Based on the detected architecture (ARM, x86, RISC-V, etc.), the AI compiles or adapts the kernel and drivers tailored to the platform.

3. **Build Disk Image or Container**

   * The AI assembles a bootable `.img`, `.iso`, or container that includes the new kernel, drivers, and shell environment.

4. **Boot Automatically**

   * The new OS version is booted directly, either on the device or inside a VM/emulator (like QEMU) for testing.

5. **Monitor Performance and Stability**

   * AI continuously checks system behavior: uptime, crashes, latency, driver responses, etc.

6. **Stable → Keep New Image**

   * If everything runs well, the system promotes this version as the active build.

7. **Unstable → Roll Back**

   * If issues are found, the AI automatically rolls back to the previous known-good state.

 
---

This workflow allows **SAGE OS to evolve, rebuild, and test itself across any platform** — moving toward true autonomy and architectural universality.
 


</details>

<details>
  <summary>⚙️ Functional Goals</summary>

- [x] Custom bootloader for Raspberry Pi
- [x] Basic kernel with memory & process management
- [x] Custom command-line shell (SAGE Shell)
- [x] Memory management system
- [x] UART driver for console I/O
- [x] Raspberry Pi 5 support
- [x] AI HAT+ driver for neural processing
- [x] Rust-based core components
- [x] Cryptography module for secure operations
- [ ] Self-tuning task scheduler
- [ ] Support for minimal file system
- [ ] Ability to evolve through version-aware updates
- [ ] Full AI model loading and inference pipeline

</details>

<details>
  <summary>🧠 AI & Machine Learning Integration</summary>

SAGE OS includes embedded, resource-efficient AI components that can:
- Perform local inference using the AI HAT+ with up to 26 TOPS
- Support multiple model formats and precisions (FP32, FP16, INT8, INT4)
- Observe usage and optimize scheduling
- Trigger self-diagnostics and reconfiguration
- Dynamically adjust power consumption based on workload
- Monitor system health and performance
- Eventually, enable modular regeneration of subsystems

The AI HAT+ provides hardware acceleration for neural networks with:
- Up to 26 TOPS of neural processing power
- 4GB of dedicated memory for AI models
- Support for various model types (classification, detection, segmentation, generation)
- Power-efficient operation with multiple power modes
- Temperature monitoring and thermal management
- High-speed data transfer using SPI and control via I2C

</details>

<details>
  <summary>🧰 Tech Stack</summary>

- **Languages**: ARM Assembly, C (kernel), Rust (core components), Python (tools & ML prototyping)
- **Platform**: Raspberry Pi 4B/5 (64-bit ARMv8/ARMv9)
- **Toolchain**: `aarch64-linux-gnu-gcc`, `rustc`, `QEMU`, `CMake`, TinyML (TFLM, uTensor)
- **Build Environment**: Cross-compilation (Linux, macOS)
- **AI Acceleration**: AI HAT+ with up to 26 TOPS neural processing

</details>

<details>
  <summary>🔐 License</summary>

This project is licensed under the [CC BY-NC 4.0 License](https://creativecommons.org/licenses/by-nc/4.0/).  
You're free to use, modify, and share the code for non-commercial purposes with attribution.  
All rights reserved to the original author.

</details>

<details>
  <summary>📦 Folder Structure</summary>

```
SAGE-OS/
├── boot/                  # Boot code
│   └── boot.S             # ARM64 boot assembly
├── kernel/                # Kernel components
│   ├── core/              # Core kernel functionality
│   │   ├── main.rs        # Rust kernel entry point
│   │   ├── init.c         # C initialization code
│   │   ├── shell.rs       # Interactive shell
│   │   └── ai_subsystem.rs # AI subsystem interface
│   ├── hal/               # Hardware abstraction layer
│   │   ├── rpi4.h         # Raspberry Pi 4 hardware definitions
│   │   └── rpi5.h         # Raspberry Pi 5 hardware definitions
│   ├── drivers/           # Hardware drivers
│   │   ├── uart.c         # UART driver
│   │   ├── gpio.c         # GPIO driver
│   │   ├── timer.c        # Timer driver
│   │   ├── ai_hat.c       # AI HAT+ driver
│   │   └── ai_hat.h       # AI HAT+ interface
│   ├── memory/            # Memory management
│   │   ├── allocator.rs   # Memory allocator
│   │   └── mmu.c          # Memory Management Unit
│   └── fs/                # File system
│       └── vfs.rs         # Virtual File System
├── security/              # Security components
│   ├── crypto.c           # Cryptography implementation
│   └── crypto.h           # Cryptography interface
├── config.txt             # Raspberry Pi 3/4 configuration
├── config_rpi5.txt        # Raspberry Pi 5 configuration
├── linker.ld              # Linker script
├── Makefile               # Build system
├── CMakeLists.txt         # CMake build configuration
├── run_qemu.sh            # QEMU runner script
├── BUILD.md               # Build instructions
└── README.md              # This file
```

</details>

<details>
  <summary>🚀 Getting Started</summary>

### Prerequisites

- Raspberry Pi 3, 4, or 5
- SD card
- USB-to-TTL serial cable
- Cross-compilation toolchain (aarch64-linux-gnu-gcc)
- Rust toolchain (for core components)
- Optional: AI HAT+ for neural processing acceleration

### Building

See [BUILD.md](BUILD.md) for detailed build instructions.

Quick start:

```bash
# Install dependencies
sudo apt-get install gcc-aarch64-linux-gnu cmake
curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh
rustup target add aarch64-unknown-none

# Clone the repository
git clone https://github.com/yourusername/SAGE-OS.git
cd SAGE-OS

# Build for Raspberry Pi 4
make

# Build for Raspberry Pi 5
make rpi5

# Build for Raspberry Pi 5 with AI HAT+ support
make rpi5_ai
```

### Running

1. Copy `kernel8.img` and the appropriate config file to an SD card:
   - For Raspberry Pi 3/4: Use `config.txt`
   - For Raspberry Pi 5: Use `config_rpi5.txt` (rename to `config.txt` on the SD card)
2. Insert the SD card into your Raspberry Pi
3. Connect a serial console
4. Power on the Raspberry Pi

### Testing with QEMU

You can test SAGE OS without physical hardware using QEMU:

```bash
# For Raspberry Pi 3/4
./run_qemu.sh

# For Raspberry Pi 5
./run_qemu.sh -p rpi5
```

### Shell Commands

Once booted, SAGE OS provides a shell with the following commands:

- `help` - Display available commands
- `echo [text]` - Echo text to the console
- `clear` - Clear the screen
- `meminfo` - Display memory information
- `reboot` - Reboot the system
- `version` - Display OS version information
- `ai info` - Display AI subsystem information (if enabled)
- `ai temp` - Show AI HAT+ temperature (if available)
- `ai power` - Show AI HAT+ power consumption (if available)
- `ai models` - List loaded AI models (if any)

</details>



```md
## 🧑‍💻 Contributing

SAGE OS is open to contributions from developers, researchers, and hardware hackers.

- 📜 [License (BSD 3-Clause)](./LICENSE)
- ⚖️ [Commercial Use Terms](./COMMERCIAL_TERMS.md)
- 🧠 [AI Safety & Ethics Manifesto](./AI_Safety_And_Ethics.md)
- 🛠️ [How to Contribute](./CONTRIBUTING.md)

By contributing, you agree to the above terms.
## License

SAGE OS is dual-licensed under the BSD 3-Clause License and a Commercial License.

You may use this project under the terms of the BSD 3-Clause License as stated in the LICENSE file.  
Alternatively, commercial use with extended rights is available — contact the author for commercial licensing.

See the [LICENSE](./LICENSE) file for details.


