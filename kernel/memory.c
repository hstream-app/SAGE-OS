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
// ─────────────────────────────────────────────────────────────────────────────#include "memory.h"
#include "uart.h"

// Simple memory allocator using a free list approach
// This is a very basic implementation for demonstration purposes

// Memory block header
typedef struct block_header {
    size_t size;                  // Size of the block (including header)
    uint8_t is_free;              // 1 if the block is free, 0 if allocated
    struct block_header* next;    // Pointer to the next block
} block_header_t;

// Memory pool
#define MEMORY_POOL_SIZE (1024 * 1024)  // 1MB memory pool
static uint8_t memory_pool[MEMORY_POOL_SIZE];
static block_header_t* free_list = NULL;

// Memory statistics
static size_t total_memory = 0;
static size_t used_memory = 0;

// Initialize memory management
void memory_init() {
    // Initialize the memory pool with a single free block
    free_list = (block_header_t*)memory_pool;
    free_list->size = MEMORY_POOL_SIZE;
    free_list->is_free = 1;
    free_list->next = NULL;
    
    total_memory = MEMORY_POOL_SIZE;
    used_memory = sizeof(block_header_t);  // Account for the initial header
    
    uart_puts("Memory management initialized\n");
    uart_printf("Total memory: %d bytes\n", total_memory);
}

// Find a free block of sufficient size
static block_header_t* find_free_block(size_t size) {
    block_header_t* current = free_list;
    
    while (current) {
        if (current->is_free && current->size >= size) {
            return current;
        }
        current = current->next;
    }
    
    return NULL;  // No suitable block found
}

// Split a block if it's too large
static void split_block(block_header_t* block, size_t size) {
    // Only split if the remaining size is large enough for a new block
    if (block->size >= size + sizeof(block_header_t) + 16) {
        block_header_t* new_block = (block_header_t*)((uint8_t*)block + size);
        new_block->size = block->size - size;
        new_block->is_free = 1;
        new_block->next = block->next;
        
        block->size = size;
        block->next = new_block;
    }
}

// Allocate memory
void* kmalloc(size_t size) {
    if (size == 0) {
        return NULL;
    }
    
    // Adjust size to include header and ensure alignment
    size_t aligned_size = (size + sizeof(block_header_t) + 7) & ~7;
    
    // Find a suitable free block
    block_header_t* block = find_free_block(aligned_size);
    if (!block) {
        uart_puts("Memory allocation failed: Out of memory\n");
        return NULL;  // Out of memory
    }
    
    // Split the block if it's too large
    split_block(block, aligned_size);
    
    // Mark the block as allocated
    block->is_free = 0;
    used_memory += block->size;
    
    // Return a pointer to the usable memory (after the header)
    return (void*)((uint8_t*)block + sizeof(block_header_t));
}

// Merge adjacent free blocks
static void merge_free_blocks() {
    block_header_t* current = free_list;
    
    while (current && current->next) {
        if (current->is_free && current->next->is_free) {
            // Merge the current block with the next one
            current->size += current->next->size;
            current->next = current->next->next;
        } else {
            current = current->next;
        }
    }
}

// Free memory
void kfree(void* ptr) {
    if (!ptr) {
        return;
    }
    
    // Get the block header
    block_header_t* block = (block_header_t*)((uint8_t*)ptr - sizeof(block_header_t));
    
    // Mark the block as free
    block->is_free = 1;
    used_memory -= block->size;
    
    // Merge adjacent free blocks
    merge_free_blocks();
}

// Get total memory size
size_t memory_total() {
    return total_memory;
}

// Get free memory size
size_t memory_free() {
    return total_memory - used_memory;
}

// Get used memory size
size_t memory_used() {
    return used_memory;
}

// Print memory statistics
void memory_stats() {
    uart_printf("Memory statistics:\n");
    uart_printf("  Total: %d bytes\n", memory_total());
    uart_printf("  Used:  %d bytes\n", memory_used());
    uart_printf("  Free:  %d bytes\n", memory_free());
    
    // Print the free list for debugging
    uart_printf("Free blocks:\n");
    block_header_t* current = free_list;
    int i = 0;
    
    while (current) {
        uart_printf("  Block %d: address=%x, size=%d, is_free=%d\n", 
                   i++, (uint32_t)current, current->size, current->is_free);
        current = current->next;
    }
}