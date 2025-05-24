<!--
─────────────────────────────────────────────────────────────────────────────
SAGE OS — Copyright (c) 2025 Ashish Vasant Yesale (ashishyesale007@gmail.com)
SPDX-License-Identifier: BSD-3-Clause OR Proprietary
SAGE OS is dual-licensed under the BSD 3-Clause License and a Commercial License.

This file is part of the SAGE OS Project.
─────────────────────────────────────────────────────────────────────────────
-->

# 🚀 SAGE OS Deployment Guide

Complete guide for deploying SAGE OS across multiple architectures and platforms.

## 📋 Table of Contents

- [Quick Deployment](#-quick-deployment)
- [Raspberry Pi Deployment](#-raspberry-pi-deployment)
- [x86_64 PC Deployment](#-x86_64-pc-deployment)
- [Virtual Machine Testing](#-virtual-machine-testing)
- [Hardware Requirements](#-hardware-requirements)
- [Troubleshooting](#-troubleshooting)

## 🚀 Quick Deployment

### 1. Build All Architectures
```bash
# Build all supported architectures
./build-all-architectures.sh

# Verify builds
ls -la build-output/
```

### 2. Test Builds
```bash
# Test all architectures with QEMU
./test-all-builds.sh

# Performance benchmarks
./benchmark-builds.sh
```

## 🥧 Raspberry Pi Deployment

### Raspberry Pi 4 (ARM64)

#### Hardware Requirements
- Raspberry Pi 4 Model B (2GB+ RAM recommended)
- MicroSD card (16GB+ Class 10)
- USB-C power supply (5V 3A)
- HDMI cable or USB-to-TTL adapter for serial console

#### Deployment Steps

##### Method 1: Direct SD Card Creation
```bash
# 1. Insert SD card and identify device
lsblk  # or diskutil list on macOS

# 2. Unmount SD card
sudo umount /dev/sdX*  # Linux
# or
diskutil unmountDisk /dev/diskN  # macOS

# 3. Create FAT32 partition
sudo fdisk /dev/sdX  # Linux
# Create new partition table, add primary partition, set type to 'c' (W95 FAT32)

# Format as FAT32
sudo mkfs.vfat /dev/sdX1  # Linux
# or
diskutil eraseDisk FAT32 SAGEOS /dev/diskN  # macOS

# 4. Mount SD card
sudo mkdir -p /mnt/sdcard
sudo mount /dev/sdX1 /mnt/sdcard

# 5. Copy SAGE OS kernel
sudo cp build-output/kernel-aarch64-rpi4.img /mnt/sdcard/kernel8.img

# 6. Create config.txt
sudo tee /mnt/sdcard/config.txt << EOF
# SAGE OS Raspberry Pi 4 Configuration
arm_64bit=1
kernel=kernel8.img
uart_2ndstage=1
enable_uart=1
core_freq=250

# Memory split (minimal GPU memory for headless)
gpu_mem=16

# Boot options
disable_splash=1
boot_delay=0

# Hardware configuration
dtparam=spi=on
dtparam=i2c_arm=on
EOF

# 7. Copy Raspberry Pi firmware (if available)
# Download from: https://github.com/raspberrypi/firmware/tree/master/boot
# sudo cp firmware/boot/* /mnt/sdcard/

# 8. Unmount and eject
sudo umount /mnt/sdcard
sudo eject /dev/sdX
```

##### Method 2: Using Raspberry Pi Imager
```bash
# 1. Install Raspberry Pi Imager
# Download from: https://www.raspberrypi.org/software/

# 2. Select "Use custom image"
# Choose: build-output/kernel-aarch64-rpi4.img

# 3. Configure advanced options (gear icon):
#    - Enable SSH: Yes
#    - Set username: sage
#    - Set password: [your-password]
#    - Configure WiFi: [your-network]
#    - Set locale: [your-locale]

# 4. Write to SD card
```

#### Serial Console Setup
```bash
# Connect USB-to-TTL adapter:
# Pin 6  (GND)  -> GND
# Pin 8  (TXD)  -> RX
# Pin 10 (RXD)  -> TX

# Connect from host computer:
screen /dev/ttyUSB0 115200    # Linux
screen /dev/cu.usbserial 115200  # macOS

# Alternative tools:
minicom -D /dev/ttyUSB0 -b 115200
picocom -b 115200 /dev/ttyUSB0
```

### Raspberry Pi 5 (ARM64)

#### Hardware Requirements
- Raspberry Pi 5 (4GB+ RAM recommended)
- MicroSD card (32GB+ Class 10 or better)
- USB-C power supply (5V 5A for Pi 5)
- AI HAT+ (optional, for AI features)

#### Deployment Steps
```bash
# Similar to Pi 4, but use Pi 5 specific kernel and config

# Copy Pi 5 kernel
sudo cp build-output/kernel-aarch64-rpi5.img /mnt/sdcard/kernel8.img

# Pi 5 specific config.txt
sudo tee /mnt/sdcard/config.txt << EOF
# SAGE OS Raspberry Pi 5 Configuration
arm_64bit=1
kernel=kernel8.img
uart_2ndstage=1
enable_uart=1

# Pi 5 performance settings
arm_boost=1
over_voltage=6
arm_freq=2400
core_freq=250

# AI HAT+ support (if available)
dtoverlay=ai-hat-plus

# Memory configuration
gpu_mem=32

# Hardware interfaces
dtparam=spi=on
dtparam=i2c_arm=on
dtparam=i2s=on
EOF
```

#### AI HAT+ Configuration
```bash
# If using AI HAT+ with Pi 5
sudo tee -a /mnt/sdcard/config.txt << EOF

# AI HAT+ Specific Configuration
dtoverlay=ai-hat-plus
dtparam=ai_accelerator=on

# Additional I2C for AI sensors
dtparam=i2c_vc=on
dtparam=i2c_arm_baudrate=400000

# SPI for AI communication
dtparam=spi=on
dtoverlay=spi1-3cs
EOF
```

## 💻 x86_64 PC Deployment

### Hardware Requirements
- x86_64 compatible PC (Intel/AMD)
- 2GB+ RAM
- USB drive (4GB+) or CD/DVD
- UEFI or Legacy BIOS support

### Creating Bootable USB
```bash
# Method 1: Direct kernel boot (GRUB required)
# 1. Format USB as FAT32
sudo mkfs.vfat /dev/sdX1

# 2. Mount USB
sudo mkdir -p /mnt/usb
sudo mount /dev/sdX1 /mnt/usb

# 3. Copy kernel
sudo cp build-output/kernel-x86_64-generic.img /mnt/usb/

# 4. Create GRUB configuration
sudo mkdir -p /mnt/usb/boot/grub
sudo tee /mnt/usb/boot/grub/grub.cfg << EOF
set timeout=5
set default=0

menuentry "SAGE OS x86_64" {
    linux /kernel-x86_64-generic.img console=ttyS0,115200 console=tty0
}
EOF

# 5. Install GRUB
sudo grub-install --target=i386-pc --boot-directory=/mnt/usb/boot /dev/sdX

# Method 2: Create ISO image
# (Requires additional tools like genisoimage or mkisofs)
```

### UEFI Boot Setup
```bash
# Create UEFI bootable USB
sudo mkdir -p /mnt/usb/EFI/BOOT

# Copy kernel
sudo cp build-output/kernel-x86_64-generic.img /mnt/usb/

# Create UEFI boot entry
sudo tee /mnt/usb/EFI/BOOT/startup.nsh << EOF
kernel-x86_64-generic.img console=ttyS0,115200
EOF
```

## 🖥️ Virtual Machine Testing

### UTM (macOS)

#### ARM64 VM Configuration
```yaml
VM Settings:
  Name: SAGE-OS-ARM64
  Architecture: ARM64 (aarch64)
  System: virt-8.2
  Memory: 2048 MB
  CPU Cores: 4
  
Boot Configuration:
  Boot from: Kernel Image
  Kernel Image: build-output/kernel-aarch64-generic.img
  Kernel Command Line: console=ttyAMA0,115200 loglevel=8
  
Network:
  Mode: Shared Network
  
Display:
  Type: Console Only
  
Serial:
  Enabled: Yes
  Mode: Built-in Terminal
```

#### x86_64 VM Configuration
```yaml
VM Settings:
  Name: SAGE-OS-x86_64
  Architecture: x86_64
  System: q35
  Memory: 2048 MB
  CPU Cores: 4
  
Boot Configuration:
  Boot from: Kernel Image
  Kernel Image: build-output/kernel-x86_64-generic.img
  Kernel Command Line: console=ttyS0,115200 console=tty0
  
Network:
  Mode: Shared Network
  
Display:
  Type: VGA
```

### QEMU (Linux/macOS)

#### ARM64 Testing
```bash
# Raspberry Pi 4 emulation
qemu-system-aarch64 \
    -M raspi3b \
    -cpu cortex-a72 \
    -m 1024 \
    -kernel build-output/kernel-aarch64-rpi4.img \
    -append "console=ttyAMA0,115200" \
    -serial stdio \
    -display none

# Generic ARM64
qemu-system-aarch64 \
    -M virt \
    -cpu cortex-a57 \
    -m 2048 \
    -kernel build-output/kernel-aarch64-generic.img \
    -append "console=ttyAMA0,115200" \
    -serial stdio \
    -display none
```

#### x86_64 Testing
```bash
# Standard PC emulation
qemu-system-x86_64 \
    -M pc \
    -cpu qemu64 \
    -m 2048 \
    -kernel build-output/kernel-x86_64-generic.img \
    -append "console=ttyS0,115200 console=tty0" \
    -serial stdio \
    -display none

# With VGA display
qemu-system-x86_64 \
    -M pc \
    -cpu qemu64 \
    -m 2048 \
    -kernel build-output/kernel-x86_64-generic.img \
    -append "console=tty0" \
    -vga std
```

### VirtualBox

#### VM Creation Script
```bash
#!/bin/bash
# Create SAGE OS VirtualBox VM

VM_NAME="SAGE-OS-x86_64"
KERNEL_PATH="$(pwd)/build-output/kernel-x86_64-generic.img"

# Create VM
VBoxManage createvm --name "$VM_NAME" --register

# Configure VM
VBoxManage modifyvm "$VM_NAME" \
    --ostype Linux_64 \
    --memory 2048 \
    --cpus 4 \
    --vram 16 \
    --boot1 disk \
    --boot2 none \
    --boot3 none \
    --boot4 none

# Create virtual disk
VBoxManage createhd --filename "$HOME/VirtualBox VMs/$VM_NAME/$VM_NAME.vdi" --size 1024

# Attach disk
VBoxManage storagectl "$VM_NAME" --name "SATA" --add sata
VBoxManage storageattach "$VM_NAME" --storagectl "SATA" --port 0 --device 0 --type hdd --medium "$HOME/VirtualBox VMs/$VM_NAME/$VM_NAME.vdi"

echo "VM created. Manual kernel boot setup required."
echo "Kernel location: $KERNEL_PATH"
```

## 🔧 Hardware Requirements

### Minimum Requirements

#### Raspberry Pi 4/5
- **CPU**: ARM Cortex-A72 (Pi 4) / Cortex-A76 (Pi 5)
- **RAM**: 1GB (minimum), 2GB+ recommended
- **Storage**: 8GB microSD (minimum), 16GB+ recommended
- **Power**: 5V 3A (Pi 4), 5V 5A (Pi 5)

#### x86_64 PC
- **CPU**: 64-bit x86 processor (Intel/AMD)
- **RAM**: 512MB (minimum), 2GB+ recommended
- **Storage**: 1GB available space
- **Boot**: UEFI or Legacy BIOS

#### Development System
- **CPU**: Multi-core processor
- **RAM**: 4GB+ for cross-compilation
- **Storage**: 10GB+ for build environment
- **OS**: Linux, macOS, or Windows with WSL2

### Recommended Hardware

#### Raspberry Pi Development
- **Model**: Raspberry Pi 5 (8GB)
- **Storage**: 32GB+ Class 10 microSD or NVMe SSD
- **Accessories**: AI HAT+, GPIO breakout, serial adapter
- **Cooling**: Active cooling for sustained performance

#### x86_64 Development
- **CPU**: Intel i5/AMD Ryzen 5 or better
- **RAM**: 8GB+ DDR4
- **Storage**: SSD for faster builds
- **Network**: Gigabit Ethernet for PXE boot

## 🔍 Troubleshooting

### Common Issues

#### Raspberry Pi Won't Boot
```bash
# Check SD card
# 1. Verify FAT32 format
# 2. Check config.txt syntax
# 3. Ensure kernel8.img is present
# 4. Try different SD card

# Debug via serial console
screen /dev/ttyUSB0 115200

# Check power supply
# Ensure adequate amperage (3A for Pi 4, 5A for Pi 5)
```

#### No Serial Output
```bash
# Verify connections:
# GND -> GND
# TX  -> RX
# RX  -> TX

# Check config.txt
enable_uart=1
uart_2ndstage=1

# Try different baud rates
screen /dev/ttyUSB0 9600
screen /dev/ttyUSB0 115200
```

#### x86_64 Boot Issues
```bash
# UEFI vs Legacy BIOS
# Ensure correct boot mode in BIOS settings

# Secure Boot
# Disable Secure Boot if enabled

# Boot order
# Set USB/CD as first boot device

# Console output
# Add console=tty0 to kernel command line
```

#### QEMU Errors
```bash
# Missing QEMU system
sudo apt-get install qemu-system-arm qemu-system-aarch64 qemu-system-x86

# Permission denied
sudo usermod -aG kvm $USER
# Log out and back in

# Network issues
# Use -netdev user,id=net0 -device virtio-net,netdev=net0
```

### Debug Commands

#### Kernel Debug
```bash
# Enable verbose output
# Add to kernel command line: loglevel=8 debug

# GDB debugging
qemu-system-aarch64 \
    -M virt \
    -kernel build-output/kernel-aarch64-generic.elf \
    -s -S \
    -serial stdio

# In another terminal
aarch64-linux-gnu-gdb build-output/kernel-aarch64-generic.elf
(gdb) target remote localhost:1234
(gdb) continue
```

#### Hardware Debug
```bash
# Check hardware detection
dmesg | grep -i sage

# Monitor system resources
top
free -h
df -h

# Network connectivity
ping 8.8.8.8
ip addr show
```

### Performance Optimization

#### Raspberry Pi
```bash
# Overclock settings (config.txt)
arm_freq=2000
gpu_freq=750
over_voltage=6

# Memory split
gpu_mem=16  # Minimal for headless

# Disable unnecessary services
systemctl disable bluetooth
systemctl disable wifi-powersave
```

#### x86_64
```bash
# CPU governor
echo performance > /sys/devices/system/cpu/cpu*/cpufreq/scaling_governor

# Disable swap
swapoff -a

# Optimize I/O scheduler
echo noop > /sys/block/sda/queue/scheduler
```

## 📊 Deployment Checklist

### Pre-Deployment
- [ ] Build all required architectures
- [ ] Test builds with QEMU
- [ ] Verify hardware compatibility
- [ ] Prepare installation media
- [ ] Configure network settings

### Raspberry Pi Deployment
- [ ] Format SD card (FAT32)
- [ ] Copy kernel image as kernel8.img
- [ ] Create config.txt
- [ ] Copy firmware files (if needed)
- [ ] Test serial console
- [ ] Verify boot process

### x86_64 Deployment
- [ ] Create bootable USB/ISO
- [ ] Configure GRUB/UEFI
- [ ] Test on target hardware
- [ ] Verify console output
- [ ] Configure network boot (if needed)

### Post-Deployment
- [ ] Verify system boot
- [ ] Test hardware functionality
- [ ] Configure network services
- [ ] Set up monitoring
- [ ] Document configuration

---

## 🎯 Quick Commands Reference

```bash
# Build all architectures
./build-all-architectures.sh

# Test all builds
./test-all-builds.sh

# Performance benchmarks
./benchmark-builds.sh

# Raspberry Pi 4 QEMU test
qemu-system-aarch64 -M raspi3b -cpu cortex-a72 -m 1024 \
    -kernel build-output/kernel-aarch64-rpi4.img -serial stdio -display none

# x86_64 QEMU test
qemu-system-x86_64 -M pc -cpu qemu64 -m 2048 \
    -kernel build-output/kernel-x86_64-generic.img -serial stdio -display none

# Serial console
screen /dev/ttyUSB0 115200

# SD card format (Linux)
sudo mkfs.vfat /dev/sdX1

# SD card format (macOS)
diskutil eraseDisk FAT32 SAGEOS /dev/diskN
```

**🚀 SAGE OS deployment made simple across all supported platforms!**