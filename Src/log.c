// Includes.

#include "log.h"

static UART_HandleTypeDef UartHandle;

// Log_Config

void Log_Config(void)
{
    GPIO_InitTypeDef         GPIO_InitStruct;
    RCC_PeriphCLKInitTypeDef RCC_PeriphClkInit;

    // Force USART1 reset.

    __USART1_FORCE_RESET();
    __USART1_RELEASE_RESET();

    // Deinit USART1 GPIO pins.

    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9);
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_10);

    // Enable GPIO TX/RX clocks.

    __GPIOA_CLK_ENABLE();
    __GPIOB_CLK_ENABLE();

    // Select SysClk as source of USART1 clocks.

    RCC_PeriphClkInit.PeriphClockSelection= RCC_PERIPHCLK_USART1;
    RCC_PeriphClkInit.Usart1ClockSelection= RCC_USART1CLKSOURCE_SYSCLK;
    HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit);

    // Enable USART1 clock.

    __USART1_CLK_ENABLE();

    // USART1 TX GPIO pin configuration.

    GPIO_InitStruct.Pin=       GPIO_PIN_9;
    GPIO_InitStruct.Mode=      GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull=      GPIO_PULLUP;
    GPIO_InitStruct.Speed=     GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate= GPIO_AF7_USART1;

    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // USART1 RX GPIO pin configuration.

    GPIO_InitStruct.Pin=       GPIO_PIN_7;
    GPIO_InitStruct.Alternate= GPIO_AF7_USART1;

    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* UART configured as follows:
      - Word Length = 8 Bits (7 data bit + 1 parity bit) :
	                  BE CAREFUL : Program 7 data bits + 1 parity bit.
      - Stop Bit    = One Stop bit
      - Parity      = ODD parity
      - BaudRate    = 115200 baud
      - Hardware flow control disabled (RTS and CTS signals) */

    UartHandle.Instance=          USART1;
    UartHandle.Init.BaudRate=     57600;
    UartHandle.Init.WordLength=   UART_WORDLENGTH_8B;
    UartHandle.Init.StopBits=     UART_STOPBITS_1;
    UartHandle.Init.Parity=       UART_PARITY_NONE;
    UartHandle.Init.HwFlowCtl=    UART_HWCONTROL_NONE;
    UartHandle.Init.Mode=         UART_MODE_TX_RX;
    UartHandle.Init.OverSampling= UART_OVERSAMPLING_16;

    if ( HAL_UART_Init(&UartHandle) != HAL_OK )
    {
        // Error

        while ( 1 )
        {
            ;;
        }
    }

    DEBUG_LOG("UART Debug console initialized");
}

// C std library printf retarget.

int _write(int   file,
           char* string,
           int   length)
{
    HAL_UART_Transmit(&UartHandle, (uint8_t *)string, length, 0xFFFF);

    return(length);
}

