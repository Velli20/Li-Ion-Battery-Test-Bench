
#ifndef __STM32F7xx_IT_H
#define __STM32F7xx_IT_H

// Includes

#include "stm32f7xx.h"

// HardFault_Handler

void HardFault_Handler(void);

// SysTick_Handler

void SysTick_Handler(void);

// ETH_IRQHandler

void ETH_IRQHandler(void);

// EXTI15_10_IRQHandler

void EXTI15_10_IRQHandler(void);

#endif 