/* ─────────────────────────────────────────────────────────────────────────────
 * SAGE OS — Copyright (c) 2025 Ashish Vasant Yesale (ashishyesale007@gmail.com)
 * SPDX-License-Identifier: BSD-3-Clause OR Proprietary
 * SAGE OS is dual-licensed under the BSD 3-Clause License and a Commercial License.
 * 
 * This file is part of the SAGE OS Project.
 * ───────────────────────────────────────────────────────────────────────────── */
#include "uart.h"

// Kernel entry point
void kernel_main() {
    // Initialize UART
    uart_init();
    
    // Print welcome message
    uart_puts("\n\n");
    uart_puts("=================================\n");
    uart_puts("  SAGE OS - Minimal Edition\n");
    uart_puts("  Hello, Raspberry Pi!\n");
    uart_puts("=================================\n\n");
    
    // Loop forever
    while (1) {
        // Echo any received characters
        char c = uart_getc();
        uart_putc(c);
    }
}