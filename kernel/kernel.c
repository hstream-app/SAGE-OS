#include "kernel.h"
#include "uart.h"
#include "memory.h"
#include "shell.h"
#include "types.h"
#include "stdio.h"

// Static buffer for version string
static char version_str[32];

// Kernel entry point
void kernel_main() {
    // Initialize hardware
    uart_init();
    
    // Display boot message
    uart_puts("\n\n");
    uart_puts("=================================\n");
    uart_puts("  SAGE OS - Self-Aware General Environment\n");
    uart_printf("  Version %s\n", kernel_version());
    uart_puts("=================================\n\n");
    
    // Initialize subsystems
    memory_init();
    shell_init();
    
    uart_puts("System initialization complete\n\n");
    
    // Run the shell
    shell_run();
    
    // Should never reach here
    kernel_panic("Shell exited unexpectedly");
}

// Kernel panic function
void kernel_panic(const char* message) {
    uart_puts("\n\n*** KERNEL PANIC ***\n");
    uart_printf("Reason: %s\n", message);
    uart_puts("System halted\n");
    
    // Halt the CPU
    while (1) {
        asm volatile("wfe");
    }
}

// Get kernel version string
const char* kernel_version() {
    // Format the version string using lightweight conversion to avoid bulky stdio
    char* p = version_str;
    *p++ = 'v';
    
    // Convert major version
    p += utoa_base(KERNEL_VERSION_MAJOR, p, 10);
    *p++ = '.';
    
    // Convert minor version
    p += utoa_base(KERNEL_VERSION_MINOR, p, 10);
    *p++ = '.';
    
    // Convert patch version
    p += utoa_base(KERNEL_VERSION_PATCH, p, 10);
    *p = '\0';
    
    return version_str;
}
