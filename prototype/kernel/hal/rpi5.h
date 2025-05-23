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
// ─────────────────────────────────────────────────────────────────────────────#ifndef RPI5_H
#define RPI5_H

#include <stdint.h>

/**
 * Raspberry Pi 5 Hardware Definitions
 * 
 * This file contains hardware-specific definitions for the Raspberry Pi 5.
 */

/* Base addresses */
#define RPI5_PERIPHERAL_BASE     0xFE000000

/* UART registers */
#define RPI5_UART0_BASE          (RPI5_PERIPHERAL_BASE + 0x201000)
#define RPI5_UART0_DR            (RPI5_UART0_BASE + 0x00)
#define RPI5_UART0_FR            (RPI5_UART0_BASE + 0x18)
#define RPI5_UART0_IBRD          (RPI5_UART0_BASE + 0x24)
#define RPI5_UART0_FBRD          (RPI5_UART0_BASE + 0x28)
#define RPI5_UART0_LCRH          (RPI5_UART0_BASE + 0x2C)
#define RPI5_UART0_CR            (RPI5_UART0_BASE + 0x30)
#define RPI5_UART0_IMSC          (RPI5_UART0_BASE + 0x38)
#define RPI5_UART0_ICR           (RPI5_UART0_BASE + 0x44)

/* GPIO registers */
#define RPI5_GPIO_BASE           (RPI5_PERIPHERAL_BASE + 0x200000)
#define RPI5_GPFSEL0             (RPI5_GPIO_BASE + 0x00)
#define RPI5_GPFSEL1             (RPI5_GPIO_BASE + 0x04)
#define RPI5_GPFSEL2             (RPI5_GPIO_BASE + 0x08)
#define RPI5_GPSET0              (RPI5_GPIO_BASE + 0x1C)
#define RPI5_GPCLR0              (RPI5_GPIO_BASE + 0x28)
#define RPI5_GPPUD               (RPI5_GPIO_BASE + 0x94)
#define RPI5_GPPUDCLK0           (RPI5_GPIO_BASE + 0x98)

/* System Timer */
#define RPI5_SYSTIMER_BASE       (RPI5_PERIPHERAL_BASE + 0x3000)
#define RPI5_SYSTIMER_CS         (RPI5_SYSTIMER_BASE + 0x00)
#define RPI5_SYSTIMER_CLO        (RPI5_SYSTIMER_BASE + 0x04)
#define RPI5_SYSTIMER_CHI        (RPI5_SYSTIMER_BASE + 0x08)
#define RPI5_SYSTIMER_C0         (RPI5_SYSTIMER_BASE + 0x0C)
#define RPI5_SYSTIMER_C1         (RPI5_SYSTIMER_BASE + 0x10)
#define RPI5_SYSTIMER_C2         (RPI5_SYSTIMER_BASE + 0x14)
#define RPI5_SYSTIMER_C3         (RPI5_SYSTIMER_BASE + 0x18)

/* Interrupt controller */
#define RPI5_INTC_BASE           (RPI5_PERIPHERAL_BASE + 0xB000)
#define RPI5_INTC_IRQ_BASIC      (RPI5_INTC_BASE + 0x200)
#define RPI5_INTC_IRQ_PEND1      (RPI5_INTC_BASE + 0x204)
#define RPI5_INTC_IRQ_PEND2      (RPI5_INTC_BASE + 0x208)
#define RPI5_INTC_IRQ_ENABLE1    (RPI5_INTC_BASE + 0x210)
#define RPI5_INTC_IRQ_ENABLE2    (RPI5_INTC_BASE + 0x214)
#define RPI5_INTC_IRQ_ENABLE_BASIC (RPI5_INTC_BASE + 0x218)

/* I2C registers for AI HAT+ communication */
#define RPI5_I2C0_BASE           (RPI5_PERIPHERAL_BASE + 0x205000)
#define RPI5_I2C1_BASE           (RPI5_PERIPHERAL_BASE + 0x804000)
#define RPI5_I2C_C               0x00
#define RPI5_I2C_S               0x04
#define RPI5_I2C_DLEN            0x08
#define RPI5_I2C_A               0x0C
#define RPI5_I2C_FIFO            0x10
#define RPI5_I2C_DIV             0x14
#define RPI5_I2C_DEL             0x18
#define RPI5_I2C_CLKT            0x1C

/* SPI registers for AI HAT+ communication */
#define RPI5_SPI0_BASE           (RPI5_PERIPHERAL_BASE + 0x204000)
#define RPI5_SPI_CS              0x00
#define RPI5_SPI_FIFO            0x04
#define RPI5_SPI_CLK             0x08
#define RPI5_SPI_DLEN            0x0C
#define RPI5_SPI_LTOH            0x10
#define RPI5_SPI_DC              0x14

/* PCIe registers for high-speed communication */
#define RPI5_PCIE_BASE           (RPI5_PERIPHERAL_BASE + 0x10000000)

/* AI HAT+ interface (custom peripheral) */
#define RPI5_AI_HAT_BASE         (RPI5_PERIPHERAL_BASE + 0x900000)
#define RPI5_AI_HAT_CTRL         (RPI5_AI_HAT_BASE + 0x00)
#define RPI5_AI_HAT_STATUS       (RPI5_AI_HAT_BASE + 0x04)
#define RPI5_AI_HAT_DATA         (RPI5_AI_HAT_BASE + 0x08)
#define RPI5_AI_HAT_CONFIG       (RPI5_AI_HAT_BASE + 0x0C)
#define RPI5_AI_HAT_POWER        (RPI5_AI_HAT_BASE + 0x10)
#define RPI5_AI_HAT_TEMP         (RPI5_AI_HAT_BASE + 0x14)
#define RPI5_AI_HAT_VERSION      (RPI5_AI_HAT_BASE + 0x18)
#define RPI5_AI_HAT_CAPS         (RPI5_AI_HAT_BASE + 0x1C)
#define RPI5_AI_HAT_MODEL_ADDR   (RPI5_AI_HAT_BASE + 0x20)
#define RPI5_AI_HAT_MODEL_SIZE   (RPI5_AI_HAT_BASE + 0x24)
#define RPI5_AI_HAT_INPUT_ADDR   (RPI5_AI_HAT_BASE + 0x28)
#define RPI5_AI_HAT_INPUT_SIZE   (RPI5_AI_HAT_BASE + 0x2C)
#define RPI5_AI_HAT_OUTPUT_ADDR  (RPI5_AI_HAT_BASE + 0x30)
#define RPI5_AI_HAT_OUTPUT_SIZE  (RPI5_AI_HAT_BASE + 0x34)
#define RPI5_AI_HAT_INFER_TIME   (RPI5_AI_HAT_BASE + 0x38)
#define RPI5_AI_HAT_CONFIDENCE   (RPI5_AI_HAT_BASE + 0x3C)
#define RPI5_AI_HAT_INT_ENABLE   (RPI5_AI_HAT_BASE + 0x40)
#define RPI5_AI_HAT_INT_STATUS   (RPI5_AI_HAT_BASE + 0x44)
#define RPI5_AI_HAT_DMA_SRC      (RPI5_AI_HAT_BASE + 0x48)
#define RPI5_AI_HAT_DMA_DST      (RPI5_AI_HAT_BASE + 0x4C)
#define RPI5_AI_HAT_DMA_LEN      (RPI5_AI_HAT_BASE + 0x50)
#define RPI5_AI_HAT_DMA_CTRL     (RPI5_AI_HAT_BASE + 0x54)

/* AI HAT+ control register bits */
#define RPI5_AI_HAT_CTRL_ENABLE      (1 << 0)
#define RPI5_AI_HAT_CTRL_RESET       (1 << 1)
#define RPI5_AI_HAT_CTRL_LOAD_MODEL  (1 << 2)
#define RPI5_AI_HAT_CTRL_START_INFER (1 << 3)
#define RPI5_AI_HAT_CTRL_ABORT       (1 << 4)
#define RPI5_AI_HAT_CTRL_DMA_ENABLE  (1 << 5)
#define RPI5_AI_HAT_CTRL_INT_ENABLE  (1 << 6)
#define RPI5_AI_HAT_CTRL_POWER_MODE  (3 << 7) /* 2 bits for power mode */

/* AI HAT+ status register bits */
#define RPI5_AI_HAT_STATUS_READY     (1 << 0)
#define RPI5_AI_HAT_STATUS_BUSY      (1 << 1)
#define RPI5_AI_HAT_STATUS_MODEL_LOADED (1 << 2)
#define RPI5_AI_HAT_STATUS_INFER_DONE (1 << 3)
#define RPI5_AI_HAT_STATUS_ERROR     (1 << 4)
#define RPI5_AI_HAT_STATUS_OVERTEMP  (1 << 5)
#define RPI5_AI_HAT_STATUS_DMA_ACTIVE (1 << 6)
#define RPI5_AI_HAT_STATUS_INT_PENDING (1 << 7)

/* AI HAT+ power modes */
#define RPI5_AI_HAT_POWER_LOW        0
#define RPI5_AI_HAT_POWER_BALANCED   1
#define RPI5_AI_HAT_POWER_HIGH       2
#define RPI5_AI_HAT_POWER_MAX        3

/* AI HAT+ model types */
#define RPI5_AI_HAT_MODEL_UNKNOWN    0
#define RPI5_AI_HAT_MODEL_CLASSIFICATION 1
#define RPI5_AI_HAT_MODEL_DETECTION  2
#define RPI5_AI_HAT_MODEL_SEGMENTATION 3
#define RPI5_AI_HAT_MODEL_GENERATION 4
#define RPI5_AI_HAT_MODEL_CUSTOM     5

/* AI HAT+ precision types */
#define RPI5_AI_HAT_PRECISION_UNKNOWN 0
#define RPI5_AI_HAT_PRECISION_FP32   1
#define RPI5_AI_HAT_PRECISION_FP16   2
#define RPI5_AI_HAT_PRECISION_INT8   3
#define RPI5_AI_HAT_PRECISION_INT4   4

/* CPU core identifiers */
#define RPI5_CORE0               0
#define RPI5_CORE1               1
#define RPI5_CORE2               2
#define RPI5_CORE3               3

/* Memory sizes and locations */
#define RPI5_MEMORY_BASE         0x00000000
#define RPI5_MEMORY_SIZE_MIN     (4 * 1024 * 1024 * 1024) /* 4GB minimum */

/* Function prototypes for UART */
void uart_init(void);
void uart_putc(unsigned char c);
unsigned char uart_getc(void);
int uart_has_data(void);
void uart_puts(const char* str);
void uart_printf(const char* format, ...);

/* Function prototypes for GPIO */
void gpio_init(void);
void gpio_set_function(uint8_t pin, uint8_t function);
void gpio_set_output(uint8_t pin, uint8_t value);
uint8_t gpio_get_input(uint8_t pin);
void gpio_set_pull(uint8_t pin, uint8_t pud);
void gpio_configure_input(uint8_t pin, uint8_t pull);
void gpio_configure_output(uint8_t pin, uint8_t initial_value);

/* Function prototypes for Timer */
void timer_init(void);
uint64_t timer_get_ticks(void);
void timer_delay_us(uint32_t us);
void timer_delay_ms(uint32_t ms);
void timer_set(uint8_t timer, uint32_t us);
uint8_t timer_triggered(uint8_t timer);
void timer_clear(uint8_t timer);

/* Function prototypes for I2C (for AI HAT+ communication) */
void i2c_init(uint8_t channel);
int i2c_write(uint8_t channel, uint8_t addr, const uint8_t *data, uint32_t len);
int i2c_read(uint8_t channel, uint8_t addr, uint8_t *data, uint32_t len);
int i2c_write_reg(uint8_t channel, uint8_t addr, uint8_t reg, uint8_t value);
int i2c_read_reg(uint8_t channel, uint8_t addr, uint8_t reg, uint8_t *value);

/* Function prototypes for SPI (for AI HAT+ communication) */
void spi_init(uint8_t channel);
int spi_transfer(uint8_t channel, const uint8_t *tx_data, uint8_t *rx_data, uint32_t len);
void spi_set_clock_speed(uint8_t channel, uint32_t speed_hz);
void spi_set_mode(uint8_t channel, uint8_t mode);

/* Function prototypes for AI HAT+ */
int ai_hat_init(void);
int ai_hat_get_version(char *version, size_t max_len);
int ai_hat_get_capabilities(float *tops, uint32_t *memory_mb);
int ai_hat_load_model(const void *model_data, size_t model_size, void *model);
int ai_hat_unload_model(void *model);
int ai_hat_run_inference(const void *model, const void *input_data, size_t input_size, void *result);
int ai_hat_free_result(void *result);
int ai_hat_get_power(uint32_t *power_mw);
int ai_hat_set_power_mode(uint8_t power_mode);
int ai_hat_get_temperature(float *temperature_c);

#endif /* RPI5_H */