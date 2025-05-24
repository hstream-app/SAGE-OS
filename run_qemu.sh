#!/bin/bash
# ─────────────────────────────────────────────────────────────────────────────
# SAGE OS — Copyright (c) 2025 Ashish Vasant Yesale (ashishyesale007@gmail.com)
# SPDX-License-Identifier: BSD-3-Clause OR Proprietary
# SAGE OS is dual-licensed under the BSD 3-Clause License and a Commercial License.
# 
# This file is part of the SAGE OS Project.
# ─────────────────────────────────────────────────────────────────────────────

# SAGE OS QEMU Runner Script
# This script helps run SAGE OS in QEMU for testing

# Check if kernel8.img exists
if [ ! -f "kernel8.img" ]; then
    echo "Error: kernel8.img not found. Build the kernel first with 'make'."
    exit 1
fi

# Default QEMU options
QEMU_CMD="qemu-system-aarch64"
QEMU_OPTS="-M raspi3 -kernel kernel8.img -serial stdio"

# Raspberry Pi model
RPI_MODEL="raspi3"

# Check if QEMU is installed
if ! command -v $QEMU_CMD &> /dev/null; then
    echo "Error: $QEMU_CMD not found. Please install QEMU."
    echo "On Debian/Ubuntu: sudo apt-get install qemu-system-arm"
    echo "On macOS: brew install qemu"
    exit 1
fi

# Parse command line arguments
DEBUG=0
GRAPHICS=0
RPI_VERSION=3

while [[ $# -gt 0 ]]; do
    case $1 in
        -d|--debug)
            DEBUG=1
            shift
            ;;
        -g|--graphics)
            GRAPHICS=1
            shift
            ;;
        -p|--platform)
            if [[ $2 == "rpi3" ]]; then
                RPI_MODEL="raspi3"
                RPI_VERSION=3
            elif [[ $2 == "rpi4" ]]; then
                RPI_MODEL="raspi3"  # QEMU doesn't have raspi4 yet, use raspi3
                RPI_VERSION=4
            elif [[ $2 == "rpi5" ]]; then
                RPI_MODEL="raspi3"  # QEMU doesn't have raspi5 yet, use raspi3
                RPI_VERSION=5
            else
                echo "Unknown platform: $2"
                echo "Supported platforms: rpi3, rpi4, rpi5"
                exit 1
            fi
            shift 2
            ;;
        -h|--help)
            echo "Usage: $0 [options]"
            echo "Options:"
            echo "  -d, --debug             Run with GDB server enabled"
            echo "  -g, --graphics          Enable graphical output (not just serial)"
            echo "  -p, --platform PLATFORM Specify platform (rpi3, rpi4, rpi5)"
            echo "  -h, --help              Show this help message"
            exit 0
            ;;
        *)
            echo "Unknown option: $1"
            echo "Use -h or --help for usage information"
            exit 1
            ;;
    esac
done

# Add debug options if requested
if [ $DEBUG -eq 1 ]; then
    QEMU_OPTS="$QEMU_OPTS -s -S"
    echo "Debug mode enabled. Connect with:"
    echo "  aarch64-linux-gnu-gdb kernel.elf -ex 'target remote localhost:1234'"
fi

# Add graphics options if requested
if [ $GRAPHICS -eq 1 ]; then
    # No special options needed, just don't use -nographic
    echo "Graphics mode enabled"
else
    QEMU_OPTS="$QEMU_OPTS -nographic"
fi

# Update QEMU options based on platform
QEMU_OPTS="-M $RPI_MODEL -kernel kernel8.img -serial stdio"

# Show platform information
echo "Platform: Raspberry Pi $RPI_VERSION"
if [ $RPI_VERSION -eq 5 ]; then
    echo "Note: QEMU doesn't fully support Raspberry Pi 5 yet, using Raspberry Pi 3 emulation"
    echo "Some features like AI HAT+ may not work correctly in emulation"
fi

# Run QEMU
echo "Starting QEMU with options: $QEMU_OPTS"
echo "Press Ctrl+A, X to exit QEMU"
echo "-----------------------------------"

$QEMU_CMD $QEMU_OPTS