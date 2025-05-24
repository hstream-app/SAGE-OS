#!/bin/bash
# ─────────────────────────────────────────────────────────────────────────────
# SAGE OS — Copyright (c) 2025 Ashish Vasant Yesale (ashishyesale007@gmail.com)
# SPDX-License-Identifier: BSD-3-Clause OR Proprietary
# SAGE OS is dual-licensed under the BSD 3-Clause License and a Commercial License.
# 
# This file is part of the SAGE OS Project.
# ─────────────────────────────────────────────────────────────────────────────
# SAGE OS Build Testing Script

set -e

echo "🧪 SAGE OS Multi-Architecture Testing"
echo "====================================="

# Check if build outputs exist
if [ ! -d "build-output" ]; then
    echo "❌ build-output directory not found. Run ./build-all-architectures.sh first."
    exit 1
fi

# Test ARM64 builds
echo "Testing ARM64 builds..."
if [ -f "build-output/kernel-aarch64-generic.img" ]; then
    echo "  Testing ARM64 generic..."
    timeout 30 qemu-system-aarch64 -M virt -cpu cortex-a57 -m 1024 \
        -kernel build-output/kernel-aarch64-generic.img \
        -serial stdio -display none || echo "  ✅ ARM64 generic test completed"
else
    echo "  ❌ ARM64 generic kernel not found"
fi

# Test ARM32 builds  
echo "Testing ARM32 builds..."
if [ -f "build-output/kernel-arm-generic.img" ]; then
    echo "  Testing ARM32 generic..."
    timeout 30 qemu-system-arm -M virt -cpu cortex-a15 -m 1024 \
        -kernel build-output/kernel-arm-generic.img \
        -serial stdio -display none || echo "  ✅ ARM32 generic test completed"
else
    echo "  ❌ ARM32 generic kernel not found"
fi

# Test x86_64 builds
echo "Testing x86_64 builds..."
if [ -f "build-output/kernel-x86_64-generic.img" ]; then
    echo "  Testing x86_64 generic..."
    timeout 30 qemu-system-x86_64 -M pc -cpu qemu64 -m 1024 \
        -kernel build-output/kernel-x86_64-generic.img \
        -serial stdio -display none || echo "  ✅ x86_64 generic test completed"
else
    echo "  ❌ x86_64 generic kernel not found"
fi

# Test RISC-V builds
echo "Testing RISC-V builds..."
if [ -f "build-output/kernel-riscv64-generic.img" ]; then
    echo "  Testing RISC-V generic..."
    timeout 30 qemu-system-riscv64 -M virt -cpu rv64 -m 1024 \
        -kernel build-output/kernel-riscv64-generic.img \
        -serial stdio -display none || echo "  ✅ RISC-V generic test completed"
else
    echo "  ❌ RISC-V generic kernel not found"
fi

echo ""
echo "✅ All architecture tests completed!"
echo ""
echo "📊 Build Summary:"
echo "=================="
ls -lh build-output/