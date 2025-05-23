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

/**
 * Initialize the system timer
 */
void timer_init(void) {
    // Nothing to do here for basic initialization
}

/**
 * Get the current timer counter value
 * 
 * @return Current timer counter value (microseconds)
 */
uint64_t timer_get_ticks(void) {
    uint32_t hi = mmio_read(RPI5_SYSTIMER_CHI);
    uint32_t lo = mmio_read(RPI5_SYSTIMER_CLO);
    
    // Check if high word changed during read
    if (hi != mmio_read(RPI5_SYSTIMER_CHI)) {
        hi = mmio_read(RPI5_SYSTIMER_CHI);
        lo = mmio_read(RPI5_SYSTIMER_CLO);
    }
    
    return ((uint64_t)hi << 32) | lo;
}

/**
 * Delay for a specified number of microseconds
 * 
 * @param us Number of microseconds to delay
 */
void timer_delay_us(uint32_t us) {
    uint64_t start = timer_get_ticks();
    uint64_t end = start + us;
    
    // Handle overflow
    if (end < start) {
        while (timer_get_ticks() >= start) {
            // Wait for overflow
        }
    }
    
    while (timer_get_ticks() < end) {
        // Wait
    }
}

/**
 * Delay for a specified number of milliseconds
 * 
 * @param ms Number of milliseconds to delay
 */
void timer_delay_ms(uint32_t ms) {
    timer_delay_us(ms * 1000);
}

/**
 * Set a timer interrupt
 * 
 * @param timer Timer number (0-3)
 * @param us Number of microseconds from now
 */
void timer_set(uint8_t timer, uint32_t us) {
    if (timer > 3) {
        return;
    }
    
    uint32_t current = mmio_read(RPI5_SYSTIMER_CLO);
    uint32_t compare = current + us;
    
    // Set the compare register
    mmio_write(RPI5_SYSTIMER_C0 + (timer * 4), compare);
    
    // Clear the timer interrupt
    mmio_write(RPI5_SYSTIMER_CS, 1 << timer);
}

/**
 * Check if a timer has triggered
 * 
 * @param timer Timer number (0-3)
 * @return 1 if triggered, 0 otherwise
 */
uint8_t timer_triggered(uint8_t timer) {
    if (timer > 3) {
        return 0;
    }
    
    return (mmio_read(RPI5_SYSTIMER_CS) & (1 << timer)) ? 1 : 0;
}

/**
 * Clear a timer interrupt
 * 
 * @param timer Timer number (0-3)
 */
void timer_clear(uint8_t timer) {
    if (timer > 3) {
        return;
    }
    
    mmio_write(RPI5_SYSTIMER_CS, 1 << timer);
}