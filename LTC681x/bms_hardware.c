// Includes

#include "bms_hardware.h"
#if defined(WITH_DEBUG_LOG)
#include "log.h"
#endif

// Static data

static TIM_HandleTypeDef TIM_TimBaseStructure;
static SPI_HandleTypeDef SpiHandle;

// DC2259A_Init

void DC2259A_Init(void)
{
    GPIO_InitTypeDef gpio_init_structure;

    // Enable chip select GPIO pin RCC clock.

    DC2259A_CHIP_SELECT_GPIO_CLK_ENABLE();

    // Initialize chip select GPIO pin.

    gpio_init_structure.Pin=   DC2259A_CHIP_SELECT_GPIO_PIN;
    gpio_init_structure.Mode=  GPIO_MODE_OUTPUT_PP;
    gpio_init_structure.Pull=  GPIO_PULLUP;
    gpio_init_structure.Speed= GPIO_SPEED_HIGH;

    HAL_GPIO_Init(DC2259A_CHIP_SELECT_GPIO_PORT, &gpio_init_structure);

    // Set chip select pin high.

    HAL_GPIO_WritePin(DC2259A_CHIP_SELECT_GPIO_PORT, DC2259A_CHIP_SELECT_GPIO_PIN, GPIO_PIN_SET);

    // Enable TIM11 RCC clock.

    __HAL_RCC_TIM11_CLK_ENABLE();

    // Initialize TIM11 peripheral.

    TIM_TimBaseStructure.Instance=           TIM11;
    TIM_TimBaseStructure.Init.Period=        59999;
	TIM_TimBaseStructure.Init.Prescaler=     215;
	TIM_TimBaseStructure.Init.ClockDivision= 0;
	TIM_TimBaseStructure.Init.CounterMode=   TIM_COUNTERMODE_UP;

    HAL_TIM_Base_Init(&TIM_TimBaseStructure);

    if ( HAL_TIM_Base_Start(&TIM_TimBaseStructure) != HAL_OK )
    {
#if defined(WITH_DEBUG_LOG)
        DEBUG_LOG("Error in starting TIM11");
#endif
    }

    // Initialize DC2259A SPI peripheral.
    // CHPA= 1, CPOL= 1
    
    SpiHandle.Instance=               DC2259A_SPI;
    SpiHandle.Init.BaudRatePrescaler= DC2259A_SPI_BAUDRATE_PRESCALER;
    SpiHandle.Init.Direction=         SPI_DIRECTION_2LINES;
    SpiHandle.Init.CLKPhase=          SPI_PHASE_2EDGE;
    SpiHandle.Init.CLKPolarity=       SPI_POLARITY_HIGH;
    SpiHandle.Init.DataSize=          SPI_DATASIZE_8BIT;
    SpiHandle.Init.FirstBit=          SPI_FIRSTBIT_MSB;
    SpiHandle.Init.TIMode=            SPI_TIMODE_DISABLE;
    SpiHandle.Init.CRCCalculation=    SPI_CRCCALCULATION_DISABLE;
    SpiHandle.Init.CRCPolynomial=     7;
    SpiHandle.Init.NSS=               SPI_NSS_HARD_OUTPUT;
    SpiHandle.Init.Mode=              SPI_MODE_MASTER;

    if ( HAL_SPI_Init(&SpiHandle) != HAL_OK )
    {
#if defined(WITH_DEBUG_LOG)
        DEBUG_LOG("DC2259A_SPI initialization failed");
#endif
    }
}

// cs_low

void cs_low(uint8_t pin)
{
    // Set chip select pin low.

    HAL_GPIO_WritePin(DC2259A_CHIP_SELECT_GPIO_PORT, DC2259A_CHIP_SELECT_GPIO_PIN, GPIO_PIN_RESET);
}

// cs_high

void cs_high(uint8_t pin)
{
    // Set chip select pin high.

    HAL_GPIO_WritePin(DC2259A_CHIP_SELECT_GPIO_PORT, DC2259A_CHIP_SELECT_GPIO_PIN, GPIO_PIN_SET);
}

// delay_u

void delay_u(uint16_t micro)
{
    // Delay...

    TIM11->CNT= 0;
    while ( (TIM11->CNT) <= micro ) 
    {
		;;
    }
}

// delay_m

void delay_m(uint16_t milli)
{
    HAL_Delay(milli);
}

// spi_write_array

void spi_write_array(uint8_t len, 
                     uint8_t data[])
{
    uint8_t ret_val;
    uint8_t i;

#if defined(WITH_SPI_DATA_LOG)
    printf("spi_write_array(len=%d) data=[", len);
#endif

    for ( i = 0; i < len; i++ )
    {
        HAL_SPI_TransmitReceive(&SpiHandle, (uint8_t*)&data[i], &ret_val, 1, HAL_MAX_DELAY);

#if defined(WITH_SPI_DATA_LOG)
        printf(i == 0 ? ("%x") : (",%x"), (uint8_t)data[i]);
#endif
    }

#if defined(WITH_SPI_DATA_LOG)   
    printf("]\n");
#endif

}

// spi_write_read

void spi_write_read(uint8_t  tx_Data[],
                    uint8_t  tx_len, 
                    uint8_t* rx_data, 
                    uint8_t  rx_len)
{
    uint8_t i;
    uint8_t data;
#if defined(WITH_SPI_DATA_LOG)
    printf("spi_write_read(tx_len=%d rx_len=%d)\n", tx_len, rx_len);
#endif

    // Transfer data to DC2259A board.

#if defined(WITH_SPI_DATA_LOG)
    printf("tx_data= [");
#endif

    for ( i = 0; i < tx_len; i++ )
    {
        // Transmit byte.

        HAL_SPI_TransmitReceive(&SpiHandle, (uint8_t*)&tx_Data[i], &data, 1, HAL_MAX_DELAY);

#if defined(WITH_SPI_DATA_LOG)
        printf(i == 0 ? ("%x") : (",%x"), (uint8_t)tx_Data[i]);
#endif
    }

#if defined(WITH_SPI_DATA_LOG)
    printf("]\n");
#endif

    // Receive data from DC2259A board.

#if defined(WITH_SPI_DATA_LOG)
    printf("rx_data= [");
#endif

    for ( i = 0; i < rx_len; i++ )
    {
        // Receive byte.

        HAL_SPI_TransmitReceive(&SpiHandle, (uint8_t*)0xFF, (uint8_t*)&rx_data[i], 1, HAL_MAX_DELAY);

#if defined(WITH_SPI_DATA_LOG)
        printf(i == 0 ? ("%x") : (",%x"), (uint8_t)rx_data[i]);
#endif
    }

#if defined(WITH_SPI_DATA_LOG)
    printf("]\n");
#endif
}

// spi_read_byte

uint8_t spi_read_byte(uint8_t tx_dat)
{
    uint8_t data;

    if ( HAL_SPI_TransmitReceive(&SpiHandle, (uint8_t*)0xFF, (uint8_t*)&data, 1, HAL_MAX_DELAY) == HAL_OK )
    {
#if defined(WITH_SPI_DATA_LOG)
        printf("spi_read_byte() data=%x\n", data);
#endif
        return(data);
    }

#if defined(WITH_SPI_DATA_LOG)
    else
    {
        DEBUG_LOG("Error in spi_read_byte()");
    }
#endif
    return(-1);
}
