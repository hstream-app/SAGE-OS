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
// ─────────────────────────────────────────────────────────────────────────────
#include "stdio.h"
#include "types.h"
#include <stdarg.h>

// Format a string and store it in a buffer
int sprintf(char* buffer, const char* format, ...) {
    va_list args;
    va_start(args, format);
    int result = vsprintf(buffer, format, args);
    va_end(args);
    return result;
}

// Helper: convert unsigned value to ASCII in given base, returns number of digits written
static int utoa_base(unsigned val, char *buf, int base) {
    static const char *digits = "0123456789abcdef";
    char *start = buf;
    // at least one digit
    do {
        *buf++ = digits[val % base];
        val /= base;
    } while (val > 0);
    // reverse [start..buf)
    for (char *p = start, *q = buf - 1; p < q; ++p, --q) {
        char t = *p; *p = *q; *q = t;
    }
    return buf - start;
}

// Helper: copy a C-string, return number of bytes (excl. NUL)
static int copy_str(char *dst, const char *src) {
    char *start = dst;
    while ((*dst++ = *src++));
    return dst - start - 1;
}

// Format a string with a va_list and store it in a buffer
int vsprintf(char* buffer, const char* format, va_list args) {
    char* buf_ptr = buffer;
    const char* fmt_ptr = format;
    
    while (*fmt_ptr) {
        if (*fmt_ptr != '%') {
            *buf_ptr++ = *fmt_ptr++;
            continue;
        }
        
        fmt_ptr++; // Skip '%'
        
        // Handle format specifiers
        switch (*fmt_ptr++) {
            case 's': {
                buf_ptr += copy_str(buf_ptr, va_arg(args, const char*));
                break;
            }
            case 'd': {
                int v = va_arg(args, int);
                if (v < 0) {
                    *buf_ptr++ = '-';
                    v = -v;
                }
                buf_ptr += utoa_base((unsigned)v, buf_ptr, 10);
                break;
            }
            case 'x': {
                unsigned v = va_arg(args, unsigned);
                *buf_ptr++ = '0';
                *buf_ptr++ = 'x';
                buf_ptr += utoa_base(v, buf_ptr, 16);
                break;
            }
            case 'c': {
                *buf_ptr++ = (char)va_arg(args, int);
                break;
            }
            case '%': {
                *buf_ptr++ = '%';
                break;
            }
            default: {
                *buf_ptr++ = *(fmt_ptr - 1);
                break;
            }
        }
    }
    
    // Null terminate
    *buf_ptr = '\0';
    
    return buf_ptr - buffer;
}