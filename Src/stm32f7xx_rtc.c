// Includes

#include "stm32f7xx_rtc.h"
#include "time.h"

// Defines

#define RTC_ASYNCH_PREDIV  0x7F
#define RTC_SYNCH_PREDIV   0x00FF

// HAL_RTC_MspDeInit

void HAL_RTC_MspDeInit(RTC_HandleTypeDef *hrtc) 
{
    __HAL_RCC_RTC_DISABLE();
}

// HAL_RTC_MspInit

void HAL_RTC_MspInit(RTC_HandleTypeDef* hrtc) 
{
	RCC_OscInitTypeDef        RCC_OscInitStruct;
    RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;

    // Enable access on RTC registers.

    __HAL_RCC_PWR_CLK_ENABLE();
    HAL_PWR_EnableBkUpAccess();

    RCC_OscInitStruct.OscillatorType= RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_LSE;
    RCC_OscInitStruct.PLL.PLLState=   RCC_PLL_NONE;
    RCC_OscInitStruct.LSIState=       RCC_LSI_ON;
    RCC_OscInitStruct.LSEState=       RCC_LSE_OFF;
  
	HAL_RCC_OscConfig(&RCC_OscInitStruct);

    PeriphClkInitStruct.PeriphClockSelection= RCC_PERIPHCLK_RTC;
    PeriphClkInitStruct.RTCClockSelection=    RCC_RTCCLKSOURCE_LSI;
  
	HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

    __HAL_RCC_RTC_ENABLE();
}

// RTC handle declaration.

static RTC_HandleTypeDef RtcHandle;

// BSP_RTC_SetDate

void rtc_set_date(uint8_t  day, 
                  uint8_t  month, 
                  uint16_t year)
{
	RTC_DateTypeDef sdatestructure;

	// Max year is 99.
	
	if ( year > 2000 )
	    year-= 2000;

	sdatestructure.Year=  year;
	sdatestructure.Month= month;
	sdatestructure.Date=  day;

	HAL_RTC_SetDate(&RtcHandle, &sdatestructure, RTC_FORMAT_BIN);

	// Writes a data in a RTC Backup data Register1.

	HAL_RTCEx_BKUPWrite(&RtcHandle, RTC_BKP_DR1, 0x32F2);
}

// rtc_set_time

void rtc_set_time(uint8_t hours, 
                  uint8_t minutes, 
                  uint8_t seconds)
{
	RTC_TimeTypeDef stimestructure;

	stimestructure.Hours=          hours;
	stimestructure.Minutes=        minutes;
	stimestructure.Seconds=        seconds;
	stimestructure.TimeFormat=     RTC_HOURFORMAT_24;
	stimestructure.DayLightSaving= RTC_DAYLIGHTSAVING_NONE ;
	stimestructure.StoreOperation= RTC_STOREOPERATION_RESET;

	HAL_RTC_SetTime(&RtcHandle, &stimestructure, RTC_FORMAT_BIN);

	// Writes a data in a RTC Backup data Register1.

	HAL_RTCEx_BKUPWrite(&RtcHandle, RTC_BKP_DR1, 0x32F2);
}

// rtc_init

void rtc_init(void) 
{
	// Configure the RTC peripheral.
	
	RtcHandle.Instance= RTC;

	RtcHandle.Init.HourFormat=     RTC_HOURFORMAT_24;
	RtcHandle.Init.AsynchPrediv=   RTC_ASYNCH_PREDIV;
	RtcHandle.Init.SynchPrediv=    RTC_SYNCH_PREDIV;
	RtcHandle.Init.OutPut=         RTC_OUTPUT_DISABLE;
	RtcHandle.Init.OutPutPolarity= RTC_OUTPUT_POLARITY_HIGH;
	RtcHandle.Init.OutPutType=     RTC_OUTPUT_TYPE_OPENDRAIN;
	
	HAL_RTC_Init(&RtcHandle);
	
	// Check if backup register is empty.

	if ( HAL_RTCEx_BKUPRead(&RtcHandle, RTC_BKP_DR1) != 0x32F2 )
	{
		rtc_set_time(0, 0, 0);
	    rtc_set_date(12, 2, 2018);
	}
}

// rtc_get_time

RTC_TimeTypeDef rtc_get_time(void) 
{
    RTC_TimeTypeDef stimestructureget;

	HAL_RTC_GetTime(&RtcHandle, &stimestructureget, RTC_FORMAT_BIN);
	return (stimestructureget);
}

// rtc_get_date

RTC_DateTypeDef rtc_get_date(void) 
{
	RTC_DateTypeDef sdatestructureget;

	HAL_RTC_GetDate(&RtcHandle, &sdatestructureget, RTC_FORMAT_BIN);
	return (sdatestructureget);
}

// time

time_t time(time_t* time_ptr)
{
	struct tm       tm_struct;
	time_t          t;

	RTC_DateTypeDef sdatestructureget;
	RTC_TimeTypeDef stimestructureget;

	sdatestructureget= rtc_get_date();
	stimestructureget= rtc_get_time();

	tm_struct.tm_sec=  stimestructureget.Seconds;
	tm_struct.tm_min=  stimestructureget.Minutes;
	tm_struct.tm_hour= stimestructureget.Hours;

	tm_struct.tm_mday= sdatestructureget.Date;
	tm_struct.tm_mon=  sdatestructureget.Month;
	tm_struct.tm_year= sdatestructureget.Year + 100;

	t= mktime(&tm_struct);
	if ( time_ptr )
	    *time_ptr= t;
	   
	return (t);
}
