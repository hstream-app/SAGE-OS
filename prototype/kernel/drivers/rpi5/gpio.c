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
 * Initialize GPIO
 */
void gpio_init(void) {
    // Nothing to do here for basic initialization
}

/**
 * Set GPIO pin function
 * 
 * @param pin GPIO pin number
 * @param function Function to set (0-7)
 */
void gpio_set_function(uint8_t pin, uint8_t function) {
    uint32_t reg_offset = (pin / 10) * 4;
    uint32_t bit = (pin % 10) * 3;
    uint32_t selector = mmio_read(RPI5_GPIO_BASE + reg_offset);
    
    selector &= ~(7 << bit);
    selector |= (function & 7) << bit;
    
    mmio_write(RPI5_GPIO_BASE + reg_offset, selector);
}

/**
 * Set GPIO pin output
 * 
 * @param pin GPIO pin number
 * @param value 0 for low, non-zero for high
 */
void gpio_set_output(uint8_t pin, uint8_t value) {
    if (value) {
        mmio_write(RPI5_GPSET0 + (pin / 32) * 4, 1 << (pin % 32));
    } else {
        mmio_write(RPI5_GPCLR0 + (pin / 32) * 4, 1 << (pin % 32));
    }
}

/**
 * Read GPIO pin input
 * 
 * @param pin GPIO pin number
 * @return 0 for low, 1 for high
 */
uint8_t gpio_get_input(uint8_t pin) {
    uint32_t value = mmio_read(RPI5_GPIO_BASE + 0x34 + (pin / 32) * 4);
    return (value & (1 << (pin % 32))) ? 1 : 0;
}

/**
 * Set GPIO pin pull-up/down
 * 
 * @param pin GPIO pin number
 * @param pud 0 for none, 1 for pull-down, 2 for pull-up
 */
void gpio_set_pull(uint8_t pin, uint8_t pud) {
    // Set the pull-up/down mode
    mmio_write(RPI5_GPPUD, pud & 3);
    
    // Wait for at least 150 cycles
    delay(150);
    
    // Clock the control signal into the GPIO pin
    mmio_write(RPI5_GPPUDCLK0 + (pin / 32) * 4, 1 << (pin % 32));
    
    // Wait for at least 150 cycles
    delay(150);
    
    // Remove the clock
    mmio_write(RPI5_GPPUDCLK0 + (pin / 32) * 4, 0);
}

/**
 * Configure GPIO pin as input
 * 
 * @param pin GPIO pin number
 * @param pull 0 for none, 1 for pull-down, 2 for pull-up
 */
void gpio_configure_input(uint8_t pin, uint8_t pull) {
    gpio_set_function(pin, 0); // Set as input
    gpio_set_pull(pin, pull);
}

/**
 * Configure GPIO pin as output
 * 
 * @param pin GPIO pin number
 * @param initial_value Initial output value (0 for low, non-zero for high)
 */
void gpio_configure_output(uint8_t pin, uint8_t initial_value) {
    gpio_set_function(pin, 1); // Set as output
    gpio_set_output(pin, initial_value);
}