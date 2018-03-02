
#ifndef BMSHARDWARE_H
#define BMSHARDWARE_H

// Includes

#include <stdio.h>
#include <stdlib.h>
#include "stm32f7xx_hal.h"

// Definations for SPI peripheral that is used to communicate with DC2259A board.

#define DC2259A_SPI                             SPI2
#define DC2259A_SPI_BAUDRATE_PRESCALER          SPI_BAUDRATEPRESCALER_64   // 1 Mhz
#define DC2259A_SPI_CLK_ENABLE()                __HAL_RCC_SPI2_CLK_ENABLE()
#define DC2259A_SPI_SCK_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOI_CLK_ENABLE()
#define DC2259A_SPI_MISO_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()
#define DC2259A_SPI_MOSI_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()
#define DC2259A_SPI_FORCE_RESET()               __HAL_RCC_SPI2_FORCE_RESET()
#define DC2259A_SPI_RELEASE_RESET()             __HAL_RCC_SPI2_RELEASE_RESET()


// Definations for DC2259A SPI GPIO pins.

#define DC2259A_SPI_SCK_PIN                     GPIO_PIN_1     // D13
#define DC2259A_SPI_SCK_GPIO_PORT               GPIOI
#define DC2259A_SPI_SCK_AF                      GPIO_AF5_SPI2
#define DC2259A_SPI_MISO_PIN                    GPIO_PIN_14    // D12
#define DC2259A_SPI_MISO_GPIO_PORT              GPIOB
#define DC2259A_SPI_MISO_AF                     GPIO_AF5_SPI2
#define DC2259A_SPI_MOSI_PIN                    GPIO_PIN_15    // D11
#define DC2259A_SPI_MOSI_GPIO_PORT              GPIOB
#define DC2259A_SPI_MOSI_AF                     GPIO_AF5_SPI2

// DC2259A chip select pin defines.

#define DC2259A_CHIP_SELECT_GPIO_PIN            GPIO_PIN_8     // D10
#define DC2259A_CHIP_SELECT_GPIO_PORT           GPIOA
#define DC2259A_CHIP_SELECT_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOA_CLK_ENABLE()

// DC2259A_Init

void DC2259A_Init(void);

// cs_low

void cs_low(uint8_t pin);

// cs_high

void cs_high(uint8_t pin);

// delay_u

void delay_u(uint16_t micro);

// delay_m

void delay_m(uint16_t milli);

// spi_write_array

void spi_write_array(uint8_t len, // Option: Number of bytes to be written on the SPI port
                     uint8_t data[] //Array of bytes to be written on the SPI port
                    );
/*
 Writes and read a set number of bytes using the SPI port.

*/

void spi_write_read(uint8_t tx_Data[],//array of data to be written on SPI port
                    uint8_t tx_len, //length of the tx data arry
                    uint8_t *rx_data,//Input: array that will store the data read by the SPI port
                    uint8_t rx_len //Option: number of bytes to be read from the SPI port
                   );

uint8_t spi_read_byte(uint8_t tx_dat);//name conflicts with linduino also needs to take a byte as a parameter
#endif