# Building and Running SAGE OS

This document provides instructions for building SAGE OS from source and running it on a Raspberry Pi.

## Prerequisites

You'll need the following tools:

- **ARM Cross-Compiler**: `aarch64-linux-gnu-gcc` or `aarch64-none-elf-gcc`
- **Rust Toolchain**: For building core components
- **CMake**: For the build system
- **Make**: For building the project
- **SD Card**: For Raspberry Pi
- **Raspberry Pi**: Model 3, 4, or 5 supported
- **Optional**: AI HAT+ for neural processing acceleration

## Building the OS

1. **Install the required tools**:

   ```bash
   # On Debian/Ubuntu
   sudo apt-get install gcc-aarch64-linux-gnu cmake
   
   # On macOS (using Homebrew)
   brew install aarch64-none-elf-gcc cmake
   ```

2. **Install Rust**:

   ```bash
   curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh
   source $HOME/.cargo/env
   rustup target add aarch64-unknown-none
   ```

3. **Clone the repository**:

   ```bash
   git clone https://github.com/yourusername/SAGE-OS.git
   cd SAGE-OS
   ```

4. **Build the kernel**:

   For Raspberry Pi 3/4:
   ```bash
   make
   ```

   For Raspberry Pi 5:
   ```bash
   make rpi5
   ```

   For Raspberry Pi 5 with AI HAT+ support:
   ```bash
   make rpi5_ai
   ```

   This will generate `kernel8.img` in the `build` directory.

## Running on Raspberry Pi

1. **Prepare the SD card**:

   - Format the SD card with FAT32 filesystem
   - Copy the following files to the SD card:
     - `kernel8.img` (from the build directory)
     - For Raspberry Pi 3/4: Copy `config.txt` from the project root
     - For Raspberry Pi 5: Copy `config_rpi5.txt` and rename it to `config.txt` on the SD card
     - Raspberry Pi firmware files (bootloader.bin, start.elf, fixup.dat)
       - You can get these from the [Raspberry Pi firmware repository](https://github.com/raspberrypi/firmware/tree/master/boot)
       - For Raspberry Pi 5, make sure to use the latest firmware files

2. **Insert the SD card** into your Raspberry Pi

3. **Connect a serial console**:
   
   - Connect a USB-to-TTL serial cable to the Raspberry Pi's GPIO pins:
     - Pin 6: GND (Ground)
     - Pin 8: GPIO14 (UART TX)
     - Pin 10: GPIO15 (UART RX)
   
   - Connect to the serial console from your computer:
     ```bash
     # On Linux/macOS
     screen /dev/ttyUSB0 115200
     
     # On Windows
     # Use PuTTY or similar terminal program
     ```

4. **Connect the AI HAT+ (if using)**:
   
   - Attach the AI HAT+ to the GPIO header of your Raspberry Pi 5
   - Ensure it's properly seated and secured
   - The AI HAT+ will be automatically detected during boot

5. **Power on the Raspberry Pi**

   You should see the SAGE OS boot messages on the serial console.

## Running in QEMU (Emulation)

You can also run SAGE OS in QEMU for testing using the provided script:

```bash
# For Raspberry Pi 3/4
./run_qemu.sh

# For Raspberry Pi 5
./run_qemu.sh -p rpi5

# With debug enabled
./run_qemu.sh -d

# With graphics enabled
./run_qemu.sh -g

# With more memory
./run_qemu.sh -m 2G
```

Or manually:

```bash
# For Raspberry Pi 3/4
qemu-system-aarch64 -M raspi3 -kernel build/kernel8.img -serial stdio -nographic

# For Raspberry Pi 5 (using raspi4 as closest model)
qemu-system-aarch64 -M raspi4 -kernel build/kernel8.img -serial stdio -nographic
```

## Debugging

For debugging with GDB:

```bash
# Using the script
./run_qemu.sh -d

# Or manually
qemu-system-aarch64 -M raspi3 -kernel build/kernel8.img -serial stdio -s -S -nographic
```

Then in another terminal:

```bash
# Connect GDB
aarch64-linux-gnu-gdb build/kernel.elf
(gdb) target remote localhost:1234
(gdb) continue
```

For Raspberry Pi 5:

```bash
# Using the script
./run_qemu.sh -p rpi5 -d

# Or manually
qemu-system-aarch64 -M raspi4 -kernel build/kernel8.img -serial stdio -s -S -nographic
```

## Troubleshooting

- **Build errors**: 
  - Make sure you have the correct cross-compiler installed
  - Ensure Rust is properly installed with the aarch64-unknown-none target
  - Check that CMake is installed and working correctly

- **Boot failures**: 
  - Check your config.txt settings
  - For Raspberry Pi 5, make sure you're using config_rpi5.txt
  - Verify you have the correct firmware files for your Raspberry Pi model

- **No serial output**: 
  - Verify your serial cable connections and terminal settings
  - Check that UART is enabled in the config.txt file

- **AI HAT+ issues**:
  - Ensure the HAT is properly connected to the GPIO header
  - Check that I2C and SPI are enabled in the config.txt file
  - Verify the HAT is receiving power (check power LED)

## Platform-Specific Notes

### Raspberry Pi 5

- The Raspberry Pi 5 has a different memory layout and peripheral addresses
- Make sure to use the rpi5 build target: `make rpi5`
- Use the config_rpi5.txt configuration file
- The AI HAT+ is only fully supported on Raspberry Pi 5

### AI HAT+

- The AI HAT+ provides up to 26 TOPS of neural processing power
- It requires I2C and SPI to be enabled
- Power consumption can be high during inference, ensure adequate cooling
- The HAT has multiple power modes that can be set via the shell:
  - `ai power low` - Low power mode (reduced performance)
  - `ai power balanced` - Balanced mode (default)
  - `ai power high` - High performance mode
  - `ai power max` - Maximum performance mode (may require additional cooling)

## Next Steps

After successfully booting SAGE OS, you can:

1. Explore the shell commands (type `help` for a list)
2. Try AI-specific commands if using the AI HAT+:
   - `ai info` - Display AI subsystem information
   - `ai temp` - Show current temperature
   - `ai power` - Show power consumption
   - `ai models` - List loaded models
3. Modify the source code to add new features
4. Contribute to the project!