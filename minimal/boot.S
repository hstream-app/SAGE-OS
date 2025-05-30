# ─────────────────────────────────────────────────────────────────────────────
# SAGE OS — Copyright (c) 2025 Ashish Vasant Yesale (ashishyesale007@gmail.com)
# SPDX-License-Identifier: BSD-3-Clause OR Proprietary
# SAGE OS is dual-licensed under the BSD 3-Clause License and a Commercial License.
# 
# This file is part of the SAGE OS Project.
# ─────────────────────────────────────────────────────────────────────────────
.section ".text.boot"
.globl _start

// Entry point for the kernel
// Processor ID (0-3) is in r0
_start:
    // Check processor ID, stop all but core 0
    mrs     x1, mpidr_el1
    and     x1, x1, #3
    cbz     x1, 2f
    // CPU ID > 0, stop
1:  wfe
    b       1b
2:  // CPU ID == 0

    // Set stack pointer
    ldr     x1, =_start
    mov     sp, x1

    // Clear BSS
    ldr     x1, =__bss_start
    ldr     x2, =__bss_end
    cmp     x1, x2
    beq     3f
    
    // Clear loop
    mov     x3, #0
1:  str     x3, [x1], #8
    cmp     x1, x2
    blo     1b
    
3:  // Jump to C code
    bl      kernel_main
    
    // If kernel_main returns, halt the CPU
hang:
    wfe
    b       hang