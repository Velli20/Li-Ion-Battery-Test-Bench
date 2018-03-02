
// Includes

#include "stm32f7xx_it.h"
#include "main.h"
#include "cmsis_os.h"

// Ethernet handle

extern ETH_HandleTypeDef EthHandle;

// HardFault_Handler

void HardFault_Handler(void)
{
    while (1)
    {
    }
}

// SysTick_Handler

void SysTick_Handler(void)
{
    osSystickHandler();
}

// ETH_IRQHandler

void ETH_IRQHandler(void)
{
    HAL_ETH_IRQHandler(&EthHandle);
}

// EXTI15_10_IRQHandler

void EXTI15_10_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(TS_INT_PIN);
}
