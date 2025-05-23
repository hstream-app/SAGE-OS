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
// ─────────────────────────────────────────────────────────────────────────────#ifndef AI_HAT_H
#define AI_HAT_H

#include <stdint.h>
#include <stddef.h>

/**
 * AI HAT+ Driver for Raspberry Pi
 * 
 * This driver provides an interface to the AI HAT+ with up to 26 TOPS
 * of neural processing power.
 */

/* AI HAT+ model types */
typedef enum {
    AI_HAT_MODEL_UNKNOWN = 0,
    AI_HAT_MODEL_CLASSIFICATION,
    AI_HAT_MODEL_DETECTION,
    AI_HAT_MODEL_SEGMENTATION,
    AI_HAT_MODEL_GENERATION,
    AI_HAT_MODEL_CUSTOM
} ai_hat_model_type_t;

/* AI HAT+ precision types */
typedef enum {
    AI_HAT_PRECISION_UNKNOWN = 0,
    AI_HAT_PRECISION_FP32,
    AI_HAT_PRECISION_FP16,
    AI_HAT_PRECISION_INT8,
    AI_HAT_PRECISION_INT4
} ai_hat_precision_t;

/* AI HAT+ model handle */
typedef struct {
    uint32_t id;
    ai_hat_model_type_t type;
    ai_hat_precision_t precision;
    size_t input_size;
    size_t output_size;
    void* private_data;
} ai_hat_model_t;

/* AI HAT+ inference result */
typedef struct {
    void* data;
    size_t size;
    uint64_t inference_time_us;
    float confidence;
} ai_hat_result_t;

/**
 * Initialize the AI HAT+
 * 
 * @return 0 on success, non-zero on failure
 */
int ai_hat_init(void);

/**
 * Get the AI HAT+ version information
 * 
 * @param version Buffer to store the version string
 * @param max_len Maximum length of the buffer
 * @return 0 on success, non-zero on failure
 */
int ai_hat_get_version(char* version, size_t max_len);

/**
 * Get the AI HAT+ capabilities
 * 
 * @param tops Pointer to store the TOPS (Tera Operations Per Second) value
 * @param memory_mb Pointer to store the available memory in MB
 * @return 0 on success, non-zero on failure
 */
int ai_hat_get_capabilities(float* tops, uint32_t* memory_mb);

/**
 * Load a model into the AI HAT+
 * 
 * @param model_data Pointer to the model data
 * @param model_size Size of the model data in bytes
 * @param model Pointer to store the model handle
 * @return 0 on success, non-zero on failure
 */
int ai_hat_load_model(const void* model_data, size_t model_size, ai_hat_model_t* model);

/**
 * Unload a model from the AI HAT+
 * 
 * @param model Pointer to the model handle
 * @return 0 on success, non-zero on failure
 */
int ai_hat_unload_model(ai_hat_model_t* model);

/**
 * Run inference on a model
 * 
 * @param model Pointer to the model handle
 * @param input_data Pointer to the input data
 * @param input_size Size of the input data in bytes
 * @param result Pointer to store the inference result
 * @return 0 on success, non-zero on failure
 */
int ai_hat_run_inference(const ai_hat_model_t* model, 
                         const void* input_data, size_t input_size,
                         ai_hat_result_t* result);

/**
 * Free an inference result
 * 
 * @param result Pointer to the inference result
 * @return 0 on success, non-zero on failure
 */
int ai_hat_free_result(ai_hat_result_t* result);

/**
 * Get the power consumption of the AI HAT+
 * 
 * @param power_mw Pointer to store the power consumption in milliwatts
 * @return 0 on success, non-zero on failure
 */
int ai_hat_get_power(uint32_t* power_mw);

/**
 * Set the power mode of the AI HAT+
 * 
 * @param power_mode Power mode (0: low power, 1: balanced, 2: high performance)
 * @return 0 on success, non-zero on failure
 */
int ai_hat_set_power_mode(uint8_t power_mode);

/**
 * Get the temperature of the AI HAT+
 * 
 * @param temperature_c Pointer to store the temperature in Celsius
 * @return 0 on success, non-zero on failure
 */
int ai_hat_get_temperature(float* temperature_c);

#endif /* AI_HAT_H */