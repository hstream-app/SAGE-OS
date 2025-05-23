ENTRY(_start)

SECTIONS
{
    /* Kernel starts at 0x80000 for Raspberry Pi 3/4 */
    . = 0x80000;
    
    __start = .;
    
    /* Boot section with startup code */
    .text.boot : {
        *(.text.boot)
    }
    
    /* Regular text section */
    .text : {
        *(.text)
    }
    
    /* Read-only data section */
    .rodata : {
        *(.rodata)
        *(.rodata.*)
    }
    
    /* Initialized data section */
    .data : {
        *(.data)
        *(.data.*)
    }
    
    /* Uninitialized data section (zeroed at boot) */
    .bss : {
        __bss_start = .;
        *(.bss)
        *(.bss.*)
        *(COMMON)
        __bss_end = .;
    }
    
    /* End of kernel */
    __end = .;
}
