#!/bin/bash
# BSD 3-Clause License
#
# Copyright (c) 2023, SAGE OS Project
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice, this
#    list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
#
# 3. Neither the name of the copyright holder nor the names of its
#    contributors may be used to endorse or promote products derived from
#    this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

# Test script for emulating SAGE OS on different architectures

set -e

ARCH=$1
TIMEOUT=30

if [ -z "$ARCH" ]; then
    echo "Usage: $0 <architecture>"
    echo "Supported architectures: x86_64, arm64, aarch64, riscv64"
    exit 1
fi

# Check if kernel image exists
if [ ! -f "build/$ARCH/kernel.img" ]; then
    echo "Error: Kernel image not found at build/$ARCH/kernel.img"
    exit 1
fi

echo "Testing SAGE OS on $ARCH architecture..."

# Set up QEMU parameters based on architecture
case $ARCH in
    x86_64)
        QEMU_CMD="qemu-system-x86_64 -kernel build/$ARCH/kernel.img -nographic -no-reboot"
        ;;
    arm64|aarch64)
        QEMU_CMD="qemu-system-aarch64 -machine virt -cpu cortex-a57 -kernel build/$ARCH/kernel.img -nographic -no-reboot"
        ;;
    riscv64)
        QEMU_CMD="qemu-system-riscv64 -machine virt -kernel build/$ARCH/kernel.img -nographic -no-reboot"
        ;;
    *)
        echo "Unsupported architecture: $ARCH"
        exit 1
        ;;
esac

# Run QEMU with timeout
echo "Running: $QEMU_CMD"
timeout $TIMEOUT $QEMU_CMD || true

echo "Test completed for $ARCH"
exit 0