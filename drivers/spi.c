// ─────────────────────────────────────────────────────────────────────────────
// SAGE OS — Copyright (c) 2025 Ashish Vasant Yesale (ashishyesale007@gmail.com)
// SPDX-License-Identifier: BSD-3-Clause OR Proprietary
// SAGE OS is dual-licensed under the BSD 3-Clause License and a Commercial License.
// 
// This file is part of the SAGE OS Project.
//
// ─────────────────────────────────────────────────────────────────────────────
#include "spi.h"
#include "uart.h"

// Raspberry Pi 5 SPI registers
#define RPI5_PERIPHERAL_BASE 0xFE000000

// SPI registers
#define SPI_BASE            (RPI5_PERIPHERAL_BASE + 0x204000)
#define SPI_CS              ((volatile uint32_t*)(SPI_BASE + 0x00))
#define SPI_FIFO            ((volatile uint32_t*)(SPI_BASE + 0x04))
#define SPI_CLK             ((volatile uint32_t*)(SPI_BASE + 0x08))
#define SPI_DLEN            ((volatile uint32_t*)(SPI_BASE + 0x0C))
#define SPI_LTOH            ((volatile uint32_t*)(SPI_BASE + 0x10))
#define SPI_DC              ((volatile uint32_t*)(SPI_BASE + 0x14))

// SPI CS register bits
#define SPI_CS_LEN_LONG     (1 << 25)
#define SPI_CS_DMA_LEN      (1 << 24)
#define SPI_CS_CSPOL2       (1 << 23)
#define SPI_CS_CSPOL1       (1 << 22)
#define SPI_CS_CSPOL0       (1 << 21)
#define SPI_CS_RXF          (1 << 20)
#define SPI_CS_RXR          (1 << 19)
#define SPI_CS_TXD          (1 << 18)
#define SPI_CS_RXD          (1 << 17)
#define SPI_CS_DONE         (1 << 16)
#define SPI_CS_LEN          (1 << 13)
#define SPI_CS_REN          (1 << 12)
#define SPI_CS_ADCS         (1 << 11)
#define SPI_CS_INTR         (1 << 10)
#define SPI_CS_INTD         (1 << 9)
#define SPI_CS_DMAEN        (1 << 8)
#define SPI_CS_TA           (1 << 7)
#define SPI_CS_CSPOL        (1 << 6)
#define SPI_CS_CLEAR_RX     (1 << 5)
#define SPI_CS_CLEAR_TX     (1 << 4)
#define SPI_CS_CPOL         (1 << 3)
#define SPI_CS_CPHA         (1 << 2)
#define SPI_CS_CS_MASK      0x3

// SPI clock divider
#define SPI_CLOCK_FREQ      250000000  // 250 MHz

// Static variables
static bool spi_initialized = false;
static spi_config_t current_config;

// Delay function - simple busy wait
static void delay(int32_t count) {
    while (count--) {
        asm volatile("nop");
    }
}

// Wait for SPI transfer to complete
static spi_status_t spi_wait_done() {
    int timeout = 1000000;  // Timeout in microseconds
    
    while (!(*SPI_CS & SPI_CS_DONE)) {
        if (--timeout <= 0) {
            return SPI_ERROR_TIMEOUT;
        }
        
        delay(10);
    }
    
    return SPI_SUCCESS;
}

// Initialize SPI controller
spi_status_t spi_init(const spi_config_t* config) {
    if (config == NULL) {
        return SPI_ERROR_PARAM;
    }
    
    uart_puts("Initializing SPI...\n");
    
    // Calculate clock divider
    uint32_t divider = SPI_CLOCK_FREQ / config->clock_speed;
    if (divider < 2) divider = 2;
    if (divider > 65536) divider = 65536;
    
    // Reset SPI controller
    *SPI_CS = 0;
    delay(100);
    
    // Clear FIFOs
    *SPI_CS = SPI_CS_CLEAR_RX | SPI_CS_CLEAR_TX;
    
    // Set clock divider
    *SPI_CLK = divider;
    
    // Configure CS polarity
    uint32_t cs_reg = 0;
    if (config->cs_pol == SPI_CS_POL_HIGH) {
        switch (config->cs_pin) {
            case 0: cs_reg |= SPI_CS_CSPOL0; break;
            case 1: cs_reg |= SPI_CS_CSPOL1; break;
            case 2: cs_reg |= SPI_CS_CSPOL2; break;
        }
    }
    
    // Configure SPI mode
    if (config->cpol == SPI_CPOL_1) {
        cs_reg |= SPI_CS_CPOL;
    }
    
    if (config->cpha == SPI_CPHA_1) {
        cs_reg |= SPI_CS_CPHA;
    }
    
    // Set chip select
    cs_reg |= (config->cs_pin & SPI_CS_CS_MASK);
    
    // Apply configuration
    *SPI_CS = cs_reg;
    
    // Save configuration
    current_config = *config;
    
    spi_initialized = true;
    uart_printf("SPI initialized at %d Hz\n", config->clock_speed);
    
    return SPI_SUCCESS;
}

// Transfer data over SPI (simultaneous read/write)
spi_status_t spi_transfer(const uint8_t* tx_data, uint8_t* rx_data, uint32_t len) {
    if (!spi_initialized) {
        return SPI_ERROR_INIT;
    }
    
    if (tx_data == NULL || rx_data == NULL || len == 0) {
        return SPI_ERROR_PARAM;
    }
    
    // Wait if SPI is busy
    if (*SPI_CS & SPI_CS_TA) {
        return SPI_ERROR_BUSY;
    }
    
    // Clear FIFOs
    *SPI_CS = *SPI_CS | SPI_CS_CLEAR_RX | SPI_CS_CLEAR_TX;
    
    // Set data length
    *SPI_DLEN = len;
    
    // Start transfer
    *SPI_CS = *SPI_CS | SPI_CS_TA;
    
    // Transfer data
    uint32_t count = 0;
    while (count < len) {
        // Write data if FIFO has space
        if ((*SPI_CS & SPI_CS_TXD) && count < len) {
            *SPI_FIFO = tx_data[count];
            count++;
        }
        
        // Read data if FIFO has data
        if (*SPI_CS & SPI_CS_RXD) {
            rx_data[count - 1] = *SPI_FIFO;
        }
    }
    
    // Wait for transfer to complete
    spi_status_t status = spi_wait_done();
    
    // Read any remaining data from FIFO
    while (*SPI_CS & SPI_CS_RXD) {
        rx_data[count - 1] = *SPI_FIFO;
        count++;
    }
    
    // End transfer
    *SPI_CS = *SPI_CS & ~SPI_CS_TA;
    
    return status;
}

// Write data over SPI (ignore received data)
spi_status_t spi_write(const uint8_t* tx_data, uint32_t len) {
    if (!spi_initialized) {
        return SPI_ERROR_INIT;
    }
    
    if (tx_data == NULL || len == 0) {
        return SPI_ERROR_PARAM;
    }
    
    // Wait if SPI is busy
    if (*SPI_CS & SPI_CS_TA) {
        return SPI_ERROR_BUSY;
    }
    
    // Clear FIFOs
    *SPI_CS = *SPI_CS | SPI_CS_CLEAR_RX | SPI_CS_CLEAR_TX;
    
    // Set data length
    *SPI_DLEN = len;
    
    // Start transfer
    *SPI_CS = *SPI_CS | SPI_CS_TA;
    
    // Write data
    uint32_t count = 0;
    while (count < len) {
        // Write data if FIFO has space
        if ((*SPI_CS & SPI_CS_TXD) && count < len) {
            *SPI_FIFO = tx_data[count];
            count++;
        }
        
        // Discard any received data
        if (*SPI_CS & SPI_CS_RXD) {
            (void)*SPI_FIFO;
        }
    }
    
    // Wait for transfer to complete
    spi_status_t status = spi_wait_done();
    
    // Discard any remaining data from FIFO
    while (*SPI_CS & SPI_CS_RXD) {
        (void)*SPI_FIFO;
    }
    
    // End transfer
    *SPI_CS = *SPI_CS & ~SPI_CS_TA;
    
    return status;
}

// Read data over SPI (send zeros)
spi_status_t spi_read(uint8_t* rx_data, uint32_t len) {
    if (!spi_initialized) {
        return SPI_ERROR_INIT;
    }
    
    if (rx_data == NULL || len == 0) {
        return SPI_ERROR_PARAM;
    }
    
    // Wait if SPI is busy
    if (*SPI_CS & SPI_CS_TA) {
        return SPI_ERROR_BUSY;
    }
    
    // Clear FIFOs
    *SPI_CS = *SPI_CS | SPI_CS_CLEAR_RX | SPI_CS_CLEAR_TX;
    
    // Set data length
    *SPI_DLEN = len;
    
    // Start transfer
    *SPI_CS = *SPI_CS | SPI_CS_TA;
    
    // Transfer data
    uint32_t count = 0;
    while (count < len) {
        // Write zeros if FIFO has space
        if ((*SPI_CS & SPI_CS_TXD) && count < len) {
            *SPI_FIFO = 0;
            count++;
        }
        
        // Read data if FIFO has data
        if (*SPI_CS & SPI_CS_RXD) {
            rx_data[count - 1] = *SPI_FIFO;
        }
    }
    
    // Wait for transfer to complete
    spi_status_t status = spi_wait_done();
    
    // Read any remaining data from FIFO
    while (*SPI_CS & SPI_CS_RXD) {
        rx_data[count - 1] = *SPI_FIFO;
        count++;
    }
    
    // End transfer
    *SPI_CS = *SPI_CS & ~SPI_CS_TA;
    
    return status;
}

// Write command and then read data over SPI
spi_status_t spi_write_read(const uint8_t* tx_data, uint32_t tx_len, uint8_t* rx_data, uint32_t rx_len) {
    spi_status_t status;
    
    // Write command
    status = spi_write(tx_data, tx_len);
    if (status != SPI_SUCCESS) {
        return status;
    }
    
    // Read data
    status = spi_read(rx_data, rx_len);
    
    return status;
}

// Set SPI clock speed
spi_status_t spi_set_clock_speed(uint32_t clock_speed) {
    if (!spi_initialized) {
        return SPI_ERROR_INIT;
    }
    
    // Calculate clock divider
    uint32_t divider = SPI_CLOCK_FREQ / clock_speed;
    if (divider < 2) divider = 2;
    if (divider > 65536) divider = 65536;
    
    // Set clock divider
    *SPI_CLK = divider;
    
    // Update current configuration
    current_config.clock_speed = clock_speed;
    
    return SPI_SUCCESS;
}

// Set SPI mode (CPOL and CPHA)
spi_status_t spi_set_mode(spi_cpol_t cpol, spi_cpha_t cpha) {
    if (!spi_initialized) {
        return SPI_ERROR_INIT;
    }
    
    // Update CS register
    uint32_t cs_reg = *SPI_CS;
    
    // Clear mode bits
    cs_reg &= ~(SPI_CS_CPOL | SPI_CS_CPHA);
    
    // Set mode bits
    if (cpol == SPI_CPOL_1) {
        cs_reg |= SPI_CS_CPOL;
    }
    
    if (cpha == SPI_CPHA_1) {
        cs_reg |= SPI_CS_CPHA;
    }
    
    // Apply configuration
    *SPI_CS = cs_reg;
    
    // Update current configuration
    current_config.cpol = cpol;
    current_config.cpha = cpha;
    
    return SPI_SUCCESS;
}