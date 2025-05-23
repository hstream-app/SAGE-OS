#!/bin/bash

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
        -h|--help)
            echo "Usage: $0 [options]"
            echo "Options:"
            echo "  -d, --debug     Run with GDB server enabled"
            echo "  -g, --graphics  Enable graphical output (not just serial)"
            echo "  -h, --help      Show this help message"
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

# Run QEMU
echo "Starting QEMU with options: $QEMU_OPTS"
echo "Press Ctrl+A, X to exit QEMU"
echo "-----------------------------------"

$QEMU_CMD $QEMU_OPTS