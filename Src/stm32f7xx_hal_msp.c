// Includes

#include "bms_hardware.h"
#include "stm32f7xx_hal.h"
#if defined(WITH_DEBUG_LOG)
#include "log.h"
#endif

// HAL_SPI_MspInit

void HAL_SPI_MspInit(SPI_HandleTypeDef* hspi)
{
    GPIO_InitTypeDef GPIO_InitStruct;

#if defined(WITH_DEBUG_LOG)
    DEBUG_LOG("HAL_SPI_MspInit()");
#endif

    // Check SPI instance.
    
    if ( !hspi || hspi->Instance != DC2259A_SPI )
    {
#if defined(WITH_DEBUG_LOG)
        DEBUG_LOG("HAL_SPI_MspInit called with incorrect SPI instance.");
#endif
    }

    // Enable DC2259A SPI peripheral GPIO TX/RX clock.

    DC2259A_SPI_SCK_GPIO_CLK_ENABLE();
    DC2259A_SPI_MISO_GPIO_CLK_ENABLE();
    DC2259A_SPI_MOSI_GPIO_CLK_ENABLE();
    
    // Enable DC2259A SPI peripheral clock.

    DC2259A_SPI_CLK_ENABLE(); 
    
    // Configure DC2259A SPI peripheral serial clock GPIO pin.

    GPIO_InitStruct.Pin=       DC2259A_SPI_SCK_PIN;
    GPIO_InitStruct.Mode=      GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull=      GPIO_PULLUP;
    GPIO_InitStruct.Speed=     GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate= DC2259A_SPI_SCK_AF;

    HAL_GPIO_Init(DC2259A_SPI_SCK_GPIO_PORT, &GPIO_InitStruct);

    // Configure DC2259A SPI peripheral miso GPIO pin.

    GPIO_InitStruct.Pin=       DC2259A_SPI_MISO_PIN;
    GPIO_InitStruct.Alternate= DC2259A_SPI_MISO_AF;
    HAL_GPIO_Init(DC2259A_SPI_MISO_GPIO_PORT, &GPIO_InitStruct);

    // Configure DC2259A SPI peripheral mosi GPIO pin.

    GPIO_InitStruct.Pin=       DC2259A_SPI_MOSI_PIN;
    GPIO_InitStruct.Alternate= DC2259A_SPI_MOSI_AF;
    HAL_GPIO_Init(DC2259A_SPI_MOSI_GPIO_PORT, &GPIO_InitStruct);
}
