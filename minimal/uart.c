/*
 * BSD 3-Clause License
 *
 * Copyright (c) 2025, Ashish Vasant Yesale
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "uart.h"

// Base address for the UART0 peripheral (BCM2835/BCM2836/BCM2837)
#define UART0_BASE 0x3F201000

// UART registers
#define UART0_DR     (UART0_BASE + 0x00) // Data Register
#define UART0_FR     (UART0_BASE + 0x18) // Flag Register
#define UART0_IBRD   (UART0_BASE + 0x24) // Integer Baud Rate Divisor
#define UART0_FBRD   (UART0_BASE + 0x28) // Fractional Baud Rate Divisor
#define UART0_LCRH   (UART0_BASE + 0x2C) // Line Control Register
#define UART0_CR     (UART0_BASE + 0x30) // Control Register
#define UART0_IMSC   (UART0_BASE + 0x38) // Interrupt Mask Set Clear Register

// Flag Register bits
#define FR_RXFE      0x10 // Receive FIFO empty
#define FR_TXFF      0x20 // Transmit FIFO full

void minimal_uart_init(void) {
    // Simplified UART initialization for minimal boot environment
    // Disable UART
    *(volatile unsigned int*)(UART0_CR) = 0;
    
    // Set baud rate - 115200 at 48MHz clock
    *(volatile unsigned int*)(UART0_IBRD) = 26;
    *(volatile unsigned int*)(UART0_FBRD) = 3;
    
    // 8 bits, no parity, 1 stop bit
    *(volatile unsigned int*)(UART0_LCRH) = 0x70;
    
    // Enable UART, transmit and receive
    *(volatile unsigned int*)(UART0_CR) = 0x301;
}

void minimal_uart_putc(char c) {
    // Wait until transmit FIFO is not full
    while (*(volatile unsigned int*)(UART0_FR) & FR_TXFF);
    
    // Write character to data register
    *(volatile unsigned int*)(UART0_DR) = c;
}

char minimal_uart_getc(void) {
    // Wait until receive FIFO is not empty
    while (*(volatile unsigned int*)(UART0_FR) & FR_RXFE);
    
    // Read character from data register
    return *(volatile unsigned int*)(UART0_DR);
}

void minimal_uart_puts(const char* str) {
    while (*str) {
        minimal_uart_putc(*str++);
    }
}