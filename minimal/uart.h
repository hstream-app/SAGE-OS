#ifndef UART_H
#define UART_H

// Initialize UART
void uart_init();

// Send a character
void uart_putc(unsigned char c);

// Receive a character
unsigned char uart_getc();

// Send a string
void uart_puts(const char* str);

#endif // UART_H