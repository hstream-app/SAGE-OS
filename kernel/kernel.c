#include "kernel.h"
#include "uart.h"

void kernel_main() {
    uart_init();
    uart_puts("SAGE OS: Boot successful.\n");
    while (1) {
        uart_puts("sage> ");
        char c = uart_getc();
        uart_putc(c); // Echo for now
    }
}
