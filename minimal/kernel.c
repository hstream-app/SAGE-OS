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