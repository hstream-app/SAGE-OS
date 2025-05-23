// ─────────────────────────────────────────────────────────────────────────────
// SAGE OS — Copyright (c) 2025 Ashish Vasant Yesale (ashishyesale007@gmail.com)
// SPDX-License-Identifier: BSD-3-Clause OR Proprietary
// SAGE OS is dual-licensed under the BSD 3-Clause License and a Commercial License.
// 
// This file is part of the SAGE OS Project.
//
// ─────────────────────────────────────────────────────────────────────────────
// Licensing:
// -----------
//                                 
//                                                                             
//   Licensed under the BSD 3-Clause License or a Commercial License.          
//   You may use this file under the terms of either license as specified in: 
//                                                                             
//      - BSD 3-Clause License (see ./LICENSE)                           
//      - Commercial License (see ./COMMERCIAL_TERMS.md or contact legal@your.org)  
//                                                                             
//   Redistribution and use in source and binary forms, with or without       
//   modification, are permitted under the BSD license provided that the      
//   following conditions are met:                                            
//                                                                             
//     * Redistributions of source code must retain the above copyright       
//       notice, this list of conditions and the following disclaimer.       
//     * Redistributions in binary form must reproduce the above copyright    
//       notice, this list of conditions and the following disclaimer in the  
//       documentation and/or other materials provided with the distribution. 
//     * Neither the name of the project nor the names of its contributors    
//       may be used to endorse or promote products derived from this         
//       software without specific prior written permission.                  
//                                                                             
//   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS  
//   IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED    
//   TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A          
//   PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER 
//   OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
//   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,      
//   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR       
//   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF   
//   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING     
//   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS       
//   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  
//
// By using this software, you agree to be bound by the terms of either license.
//
// Alternatively, commercial use with extended rights is available — contact the author for commercial licensing.
//
// ─────────────────────────────────────────────────────────────────────────────
// Contributor Guidelines:
// ------------------------
// Contributions are welcome under the terms of the Developer Certificate of Origin (DCO).
// All contributors must certify that they have the right to submit the code and agree to
// release it under the above license terms.
//
// Contributions must:
//   - Be original or appropriately attributed
//   - Include clear documentation and test cases where applicable
//   - Respect the coding and security guidelines defined in CONTRIBUTING.md
//
// ─────────────────────────────────────────────────────────────────────────────
// Terms of Use and Disclaimer:
// -----------------------------
// This software is provided "as is", without any express or implied warranty.
// In no event shall the authors, contributors, or copyright holders
// be held liable for any damages arising from the use of this software.
//
// Use of this software in critical systems (e.g., medical, nuclear, safety)
// is entirely at your own risk unless specifically licensed for such purposes.
//
// ─────────────────────────────────────────────────────────────────────────────#include <stdint.h>
#include <stddef.h>
#include "../../hal/rpi5.h"

// Memory-mapped I/O access
static inline void mmio_write(uint64_t reg, uint32_t data) {
    *(volatile uint32_t*)reg = data;
}

static inline uint32_t mmio_read(uint64_t reg) {
    return *(volatile uint32_t*)reg;
}

// Delay function
static void delay(int32_t count) {
    asm volatile("__delay_%=: subs %[count], %[count], #1; bne __delay_%=\n"
                 : "=r"(count)
                 : [count] "0"(count)
                 : "cc");
}

/**
 * Initialize the UART
 */
void uart_init(void) {
    // Disable UART0
    mmio_write(RPI5_UART0_CR, 0x00000000);
    
    // Setup GPIO pins 14 and 15
    uint32_t selector = mmio_read(RPI5_GPFSEL1);
    selector &= ~(7 << 12);  // Clean GPIO14
    selector |= 4 << 12;     // Set GPIO14 to alt0 (UART0_TXD)
    selector &= ~(7 << 15);  // Clean GPIO15
    selector |= 4 << 15;     // Set GPIO15 to alt0 (UART0_RXD)
    mmio_write(RPI5_GPFSEL1, selector);
    
    // Disable GPIO pull-up/down
    mmio_write(RPI5_GPPUD, 0x00000000);
    delay(150);
    mmio_write(RPI5_GPPUDCLK0, (1 << 14) | (1 << 15));
    delay(150);
    mmio_write(RPI5_GPPUDCLK0, 0x00000000);
    
    // Clear pending interrupts
    mmio_write(RPI5_UART0_ICR, 0x7FF);
    
    // Set integer & fractional part of baud rate
    // Divider = UART_CLOCK/(16 * Baud)
    // UART_CLOCK = 48MHz for Raspberry Pi 5
    // Baud = 115200
    // Divider = 48000000 / (16 * 115200) = 26.0416666667
    // Integer part = 26
    // Fractional part = (.0416666667 * 64) = 3
    mmio_write(RPI5_UART0_IBRD, 26);
    mmio_write(RPI5_UART0_FBRD, 3);
    
    // Enable FIFO & 8-bit data transmission (1 stop bit, no parity)
    mmio_write(RPI5_UART0_LCRH, (1 << 4) | (1 << 5) | (1 << 6));
    
    // Mask all interrupts
    mmio_write(RPI5_UART0_IMSC, (1 << 1) | (1 << 4) | (1 << 5) | (1 << 6) |
                               (1 << 7) | (1 << 8) | (1 << 9) | (1 << 10));
    
    // Enable UART0, receive & transfer part of UART
    mmio_write(RPI5_UART0_CR, (1 << 0) | (1 << 8) | (1 << 9));
}

/**
 * Send a character to the UART
 */
void uart_putc(unsigned char c) {
    // Wait for UART to become ready to transmit
    while (mmio_read(RPI5_UART0_FR) & (1 << 5)) {}
    mmio_write(RPI5_UART0_DR, c);
}

/**
 * Receive a character from the UART
 */
unsigned char uart_getc(void) {
    // Wait for UART to have received something
    while (mmio_read(RPI5_UART0_FR) & (1 << 4)) {}
    return mmio_read(RPI5_UART0_DR);
}

/**
 * Check if UART has received any data
 */
int uart_has_data(void) {
    return !(mmio_read(RPI5_UART0_FR) & (1 << 4));
}

/**
 * Send a string to the UART
 */
void uart_puts(const char* str) {
    for (size_t i = 0; str[i] != '\0'; i++) {
        uart_putc((unsigned char)str[i]);
    }
}

/**
 * Send a formatted string to the UART
 */
void uart_printf(const char* format, ...) {
    // Simple implementation - just forward to puts for now
    uart_puts(format);
}