#include "uart.h"
#include <stdint.h>

#define UART0_BASE 0x3F201000

void uart_init() {
    // Simplified UART init for early output
}

void uart_putc(char c) {
    *(volatile unsigned int*)(UART0_BASE + 0x00) = c;
}

char uart_getc() {
    return *(volatile unsigned int*)(UART0_BASE + 0x00);
}

void uart_puts(const char* str) {
    while (*str) uart_putc(*str++);
}
