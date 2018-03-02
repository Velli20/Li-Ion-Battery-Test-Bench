#ifndef __STM32F7XX_RTC_H
#define __STM32F7XX_RTC_H

// Includes

#include "stm32f7xx_hal.h"

// rtc_set_date

void rtc_set_date(uint8_t  day, 
                  uint8_t  month, 
                  uint16_t year);

// rtc_set_time

void rtc_set_time(uint8_t hours, 
                  uint8_t minutes, 
                  uint8_t seconds);

// rtc_init

void rtc_init(void);

// rtc_get_time

RTC_TimeTypeDef rtc_get_time(void);

// rtc_get_date

RTC_DateTypeDef rtc_get_date(void);

#endif
