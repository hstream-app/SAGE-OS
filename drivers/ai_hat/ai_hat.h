/* ─────────────────────────────────────────────────────────────────────────────
 * SAGE OS — Copyright (c) 2025 Ashish Vasant Yesale (ashishyesale007@gmail.com)
 * SPDX-License-Identifier: BSD-3-Clause OR Proprietary
 * SAGE OS is dual-licensed under the BSD 3-Clause License and a Commercial License.
 * 
 * This file is part of the SAGE OS Project.
 *
 * ───────────────────────────────────────────────────────────────────────────── */
#ifndef AI_HAT_H
#define AI_HAT_H

#include "../../kernel/types.h"

// AI HAT+ status codes
typedef enum {
    AI_HAT_SUCCESS = 0,
    AI_HAT_ERROR_INIT = -1,
    AI_HAT_ERROR_COMM = -2,
    AI_HAT_ERROR_PARAM = -3,
    AI_HAT_ERROR_MODEL = -4,
    AI_HAT_ERROR_MEMORY = -5,
    AI_HAT_ERROR_TIMEOUT = -6
} ai_hat_status_t;

// AI HAT+ power modes
typedef enum {
    AI_HAT_POWER_OFF = 0,
    AI_HAT_POWER_LOW = 1,
    AI_HAT_POWER_MEDIUM = 2,
    AI_HAT_POWER_HIGH = 3,
    AI_HAT_POWER_MAX = 4
} ai_hat_power_mode_t;

// AI HAT+ model precision
typedef enum {
    AI_HAT_PRECISION_FP32 = 0,
    AI_HAT_PRECISION_FP16 = 1,
    AI_HAT_PRECISION_INT8 = 2,
    AI_HAT_PRECISION_INT4 = 3
} ai_hat_precision_t;

// AI HAT+ information structure
typedef struct {
    uint32_t version;
    uint32_t max_tops;
    uint32_t memory_size;
    uint32_t temperature;
    uint32_t power_consumption;
    ai_hat_power_mode_t power_mode;
} ai_hat_info_t;

// AI HAT+ model information
typedef struct {
    char name[32];
    uint32_t id;
    uint32_t size;
    ai_hat_precision_t precision;
    uint32_t input_size;
    uint32_t output_size;
} ai_hat_model_t;

// Initialize the AI HAT+
ai_hat_status_t ai_hat_init(void);

// Get AI HAT+ information
ai_hat_status_t ai_hat_get_info(ai_hat_info_t* info);

// Set AI HAT+ power mode
ai_hat_status_t ai_hat_set_power_mode(ai_hat_power_mode_t mode);

// Get AI HAT+ temperature (in Celsius)
ai_hat_status_t ai_hat_get_temperature(uint32_t* temperature);

// Get AI HAT+ power consumption (in mW)
ai_hat_status_t ai_hat_get_power_consumption(uint32_t* power);

// Load a model to the AI HAT+
ai_hat_status_t ai_hat_load_model(const void* model_data, uint32_t model_size, uint32_t* model_id);

// Unload a model from the AI HAT+
ai_hat_status_t ai_hat_unload_model(uint32_t model_id);

// Run inference on a loaded model
ai_hat_status_t ai_hat_run_inference(uint32_t model_id, const void* input, uint32_t input_size, void* output, uint32_t output_size);

// Get list of loaded models
ai_hat_status_t ai_hat_get_models(ai_hat_model_t* models, uint32_t max_models, uint32_t* num_models);

// Shutdown the AI HAT+
void ai_hat_shutdown(void);

#endif // AI_HAT_H