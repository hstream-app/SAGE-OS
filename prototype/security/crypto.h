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
// ─────────────────────────────────────────────────────────────────────────────#ifndef CRYPTO_H
#define CRYPTO_H

#include <stdint.h>
#include <stddef.h>

/**
 * Initialize the cryptography subsystem
 * 
 * @return 0 on success, non-zero on failure
 */
int crypto_init(void);

/**
 * Generate a random buffer
 * 
 * @param buffer Pointer to the buffer to fill with random data
 * @param size Size of the buffer in bytes
 * @return 0 on success, non-zero on failure
 */
int crypto_random_bytes(uint8_t* buffer, size_t size);

/**
 * Hash a buffer using SHA-256
 * 
 * @param data Pointer to the data to hash
 * @param data_size Size of the data in bytes
 * @param hash Pointer to a 32-byte buffer to store the hash
 * @return 0 on success, non-zero on failure
 */
int crypto_sha256(const uint8_t* data, size_t data_size, uint8_t hash[32]);

/**
 * Encrypt a buffer using AES-256-GCM
 * 
 * @param plaintext Pointer to the plaintext data
 * @param plaintext_size Size of the plaintext in bytes
 * @param key Pointer to the 32-byte key
 * @param iv Pointer to the 12-byte initialization vector
 * @param ciphertext Pointer to the buffer to store the ciphertext
 * @param ciphertext_size Size of the ciphertext buffer in bytes
 * @param tag Pointer to the 16-byte buffer to store the authentication tag
 * @return 0 on success, non-zero on failure
 */
int crypto_aes_encrypt(const uint8_t* plaintext, size_t plaintext_size,
                       const uint8_t key[32], const uint8_t iv[12],
                       uint8_t* ciphertext, size_t ciphertext_size,
                       uint8_t tag[16]);

/**
 * Decrypt a buffer using AES-256-GCM
 * 
 * @param ciphertext Pointer to the ciphertext data
 * @param ciphertext_size Size of the ciphertext in bytes
 * @param key Pointer to the 32-byte key
 * @param iv Pointer to the 12-byte initialization vector
 * @param tag Pointer to the 16-byte authentication tag
 * @param plaintext Pointer to the buffer to store the plaintext
 * @param plaintext_size Size of the plaintext buffer in bytes
 * @return 0 on success, non-zero on failure
 */
int crypto_aes_decrypt(const uint8_t* ciphertext, size_t ciphertext_size,
                       const uint8_t key[32], const uint8_t iv[12],
                       const uint8_t tag[16],
                       uint8_t* plaintext, size_t plaintext_size);

/**
 * Generate an Ed25519 key pair
 * 
 * @param public_key Pointer to a 32-byte buffer to store the public key
 * @param private_key Pointer to a 64-byte buffer to store the private key
 * @return 0 on success, non-zero on failure
 */
int crypto_ed25519_keygen(uint8_t public_key[32], uint8_t private_key[64]);

/**
 * Sign a message using Ed25519
 * 
 * @param message Pointer to the message to sign
 * @param message_size Size of the message in bytes
 * @param private_key Pointer to the 64-byte private key
 * @param signature Pointer to a 64-byte buffer to store the signature
 * @return 0 on success, non-zero on failure
 */
int crypto_ed25519_sign(const uint8_t* message, size_t message_size,
                        const uint8_t private_key[64],
                        uint8_t signature[64]);

/**
 * Verify a signature using Ed25519
 * 
 * @param message Pointer to the message
 * @param message_size Size of the message in bytes
 * @param public_key Pointer to the 32-byte public key
 * @param signature Pointer to the 64-byte signature
 * @return 1 if the signature is valid, 0 if invalid, negative on error
 */
int crypto_ed25519_verify(const uint8_t* message, size_t message_size,
                          const uint8_t public_key[32],
                          const uint8_t signature[64]);

#endif /* CRYPTO_H */