#![no_std]
#![no_main]
#![feature(alloc_error_handler)]
#![feature(asm_const)]

extern crate alloc;

use core::panic::PanicInfo;
use sage_os::{self, console, hal, shell};

#[no_mangle]
pub extern "C" fn kernel_main() -> ! {
    // Initialize the kernel
    sage_os::init();
    
    // Print welcome message
    console::println!("SAGE OS - Self-Aware General Environment");
    console::println!("----------------------------------------");
    console::println!("Initializing kernel...");
    
    // Initialize shell
    shell::init();
    
    // Run the shell
    console::println!("Initialization complete. Starting shell...");
    shell::run();
    
    // We should never reach here
    loop {
        hal::halt();
    }
}

#[panic_handler]
fn panic(info: &PanicInfo) -> ! {
    console::println!("KERNEL PANIC: {}", info);
    loop {
        // Halt the CPU
        hal::halt();
    }
}