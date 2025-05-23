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
// ─────────────────────────────────────────────────────────────────────────────#include "crypto.h"

#ifdef ENABLE_CRYPTO

#include <string.h>

// External dependencies
#include "sodium.h"

int crypto_init(void) {
    if (sodium_init() < 0) {
        return -1;
    }
    return 0;
}

int crypto_random_bytes(uint8_t* buffer, size_t size) {
    randombytes_buf(buffer, size);
    return 0;
}

int crypto_sha256(const uint8_t* data, size_t data_size, uint8_t hash[32]) {
    crypto_hash_sha256_state state;
    crypto_hash_sha256_init(&state);
    crypto_hash_sha256_update(&state, data, data_size);
    crypto_hash_sha256_final(&state, hash);
    return 0;
}

int crypto_aes_encrypt(const uint8_t* plaintext, size_t plaintext_size,
                       const uint8_t key[32], const uint8_t iv[12],
                       uint8_t* ciphertext, size_t ciphertext_size,
                       uint8_t tag[16]) {
    if (ciphertext_size < plaintext_size) {
        return -1;
    }
    
    return crypto_aead_aes256gcm_encrypt(
        ciphertext, NULL,
        plaintext, plaintext_size,
        NULL, 0,
        NULL, iv, key
    );
}

int crypto_aes_decrypt(const uint8_t* ciphertext, size_t ciphertext_size,
                       const uint8_t key[32], const uint8_t iv[12],
                       const uint8_t tag[16],
                       uint8_t* plaintext, size_t plaintext_size) {
    if (plaintext_size < ciphertext_size) {
        return -1;
    }
    
    unsigned long long actual_size;
    return crypto_aead_aes256gcm_decrypt(
        plaintext, &actual_size,
        NULL,
        ciphertext, ciphertext_size,
        NULL, 0,
        iv, key
    );
}

int crypto_ed25519_keygen(uint8_t public_key[32], uint8_t private_key[64]) {
    return crypto_sign_ed25519_keypair(public_key, private_key);
}

int crypto_ed25519_sign(const uint8_t* message, size_t message_size,
                        const uint8_t private_key[64],
                        uint8_t signature[64]) {
    unsigned long long signature_len;
    unsigned char signed_message[message_size + 64];
    
    int result = crypto_sign_ed25519(
        signed_message, &signature_len,
        message, message_size,
        private_key
    );
    
    if (result != 0) {
        return result;
    }
    
    // Extract the signature (first 64 bytes of signed_message)
    memcpy(signature, signed_message, 64);
    
    return 0;
}

int crypto_ed25519_verify(const uint8_t* message, size_t message_size,
                          const uint8_t public_key[32],
                          const uint8_t signature[64]) {
    // Construct the signed message (signature + message)
    unsigned char signed_message[message_size + 64];
    memcpy(signed_message, signature, 64);
    memcpy(signed_message + 64, message, message_size);
    
    unsigned char verified_message[message_size];
    unsigned long long verified_message_len;
    
    return crypto_sign_ed25519_open(
        verified_message, &verified_message_len,
        signed_message, message_size + 64,
        public_key
    ) == 0 ? 1 : 0;
}

#else // ENABLE_CRYPTO

// Stub implementations when cryptography is disabled

int crypto_init(void) {
    return -1;
}

int crypto_random_bytes(uint8_t* buffer, size_t size) {
    // Fill with predictable pattern for testing
    for (size_t i = 0; i < size; i++) {
        buffer[i] = (uint8_t)i;
    }
    return 0;
}

int crypto_sha256(const uint8_t* data, size_t data_size, uint8_t hash[32]) {
    // Fill with zeros
    memset(hash, 0, 32);
    return -1;
}

int crypto_aes_encrypt(const uint8_t* plaintext, size_t plaintext_size,
                       const uint8_t key[32], const uint8_t iv[12],
                       uint8_t* ciphertext, size_t ciphertext_size,
                       uint8_t tag[16]) {
    if (ciphertext_size < plaintext_size) {
        return -1;
    }
    
    // Just copy the plaintext
    memcpy(ciphertext, plaintext, plaintext_size);
    memset(tag, 0, 16);
    
    return 0;
}

int crypto_aes_decrypt(const uint8_t* ciphertext, size_t ciphertext_size,
                       const uint8_t key[32], const uint8_t iv[12],
                       const uint8_t tag[16],
                       uint8_t* plaintext, size_t plaintext_size) {
    if (plaintext_size < ciphertext_size) {
        return -1;
    }
    
    // Just copy the ciphertext
    memcpy(plaintext, ciphertext, ciphertext_size);
    
    return 0;
}

int crypto_ed25519_keygen(uint8_t public_key[32], uint8_t private_key[64]) {
    // Fill with zeros
    memset(public_key, 0, 32);
    memset(private_key, 0, 64);
    return -1;
}

int crypto_ed25519_sign(const uint8_t* message, size_t message_size,
                        const uint8_t private_key[64],
                        uint8_t signature[64]) {
    // Fill with zeros
    memset(signature, 0, 64);
    return -1;
}

int crypto_ed25519_verify(const uint8_t* message, size_t message_size,
                          const uint8_t public_key[32],
                          const uint8_t signature[64]) {
    return -1;
}

#endif // ENABLE_CRYPTO