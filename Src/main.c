
// Includes

#include "cmsis_os.h"
#include "ethernetif.h"
#include "log.h"
#include "main.h"
#include "stm32f7xx_rtc.h"
#include "ui.h"

// Variables

osMutexId mass_storage_mutex;

// Error_Handler

static void Error_Handler(void)
{    
    while ( 1 )
    {
        ;;
    }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 200000000
  *            HCLK(Hz)                       = 200000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 25
  *            PLL_N                          = 432
  *            PLL_P                          = 2
  *            PLL_Q                          = 9
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 7
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_OscInitTypeDef RCC_OscInitStruct;

    // Enable HSE Oscillator and activate PLL with HSE as source.

    RCC_OscInitStruct.OscillatorType= RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState=       RCC_HSE_ON;
    RCC_OscInitStruct.HSIState=       RCC_HSI_OFF;
    RCC_OscInitStruct.PLL.PLLState=   RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource=  RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM=       25;
    RCC_OscInitStruct.PLL.PLLN=       400;
    RCC_OscInitStruct.PLL.PLLP=       RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ=       9;

    if ( HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK )
        Error_Handler();

    // Activate the OverDrive.

    if ( HAL_PWREx_EnableOverDrive() != HAL_OK )
        Error_Handler();
  
    // Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers.

    RCC_ClkInitStruct.ClockType=      (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStruct.SYSCLKSource=   RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider=  RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider= RCC_HCLK_DIV4;  
    RCC_ClkInitStruct.APB2CLKDivider= RCC_HCLK_DIV2;  

    if ( HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK )
        Error_Handler();
}

// CPU_CACHE_Enable

static void CPU_CACHE_Enable(void)
{
    // Enable I-Cache.

    SCB_EnableICache();

    // Enable D-Cache.

    SCB_EnableDCache();
}

// MPU_Config

static void MPU_Config(void)
{
    MPU_Region_InitTypeDef MPU_InitStruct;
  
    // Disable the MPU.

    HAL_MPU_Disable();
  
    // Configure the MPU attributes as Device for Ethernet Descriptors in the SRAM.

    MPU_InitStruct.Enable=           MPU_REGION_ENABLE;
    MPU_InitStruct.BaseAddress=      0x20010000;
    MPU_InitStruct.Size=             MPU_REGION_SIZE_256B;
    MPU_InitStruct.AccessPermission= MPU_REGION_FULL_ACCESS;
    MPU_InitStruct.IsBufferable=     MPU_ACCESS_BUFFERABLE;
    MPU_InitStruct.IsCacheable=      MPU_ACCESS_NOT_CACHEABLE;
    MPU_InitStruct.IsShareable=      MPU_ACCESS_SHAREABLE;
    MPU_InitStruct.Number=           MPU_REGION_NUMBER0;
    MPU_InitStruct.TypeExtField=     MPU_TEX_LEVEL0;
    MPU_InitStruct.SubRegionDisable= 0x00;
    MPU_InitStruct.DisableExec=      MPU_INSTRUCTION_ACCESS_ENABLE;

    HAL_MPU_ConfigRegion(&MPU_InitStruct);

    // Enable the MPU.

    HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

// main

int main(void)
{
    // Configure the MPU attributes as Device memory for ETH DMA descriptors.

    MPU_Config();

    // Enable the CPU Cache.

    CPU_CACHE_Enable();

    // HAL library initialization.

    HAL_Init();  
  
    // Configure the system clock.

    SystemClock_Config(); 

    // Initialize the NOR.

    BSP_QSPI_Init();

    // Configure the NOR in memory mapped mode.
    
    BSP_QSPI_EnableMemoryMappedMode();

    // Initialize the UART log.

    Log_Config();

    // Initialize the Real Time Clock.

    rtc_init();

    // Initialize mass storage mutex.
    
    osMutexDef(sd_mutex);
    mass_storage_mutex= osMutexCreate(osMutex(sd_mutex));

    // Init main thread.

    osThreadDef(Main, main_thread, osPriorityBelowNormal, 0, configMINIMAL_STACK_SIZE * 20);
    osThreadCreate(osThread(Main), NULL);

    // Start scheduler.

    osKernelStart();
  
    for( ;; );
}



#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif
