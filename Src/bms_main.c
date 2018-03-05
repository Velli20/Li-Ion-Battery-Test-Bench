
// Includes

#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "stm32f7xx_rtc.h"

// Includes for data log to file

#include "ff_gen_drv.h"
#include "sd_diskio.h"
#include "stm32746g_discovery.h"
#include "stm32746g_discovery_sd.h"

#if defined(WITH_DEBUG_LOG)
#include "log.h"
#endif

// Macros

#define MAX(x, y)(x >= y ? y : x)
#define MIN(x, y)(x <  y ? y : x)

// Pin definations

// ARD_D9

#define BMS_DISCHARGE_LOAD_1_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define BMS_DISCHARGE_LOAD_1_GPIO_PORT         GPIOA
#define BMS_DISCHARGE_LOAD_1_GPIO_PIN          GPIO_PIN_15

// ARD_D8

#define BMS_DISCHARGE_LOAD_2_GPIO_CLK_ENABLE() __HAL_RCC_GPIOI_CLK_ENABLE()
#define BMS_DISCHARGE_LOAD_2_GPIO_PORT         GPIOI
#define BMS_DISCHARGE_LOAD_2_GPIO_PIN          GPIO_PIN_2

// ARD_D7

#define BMS_DISCHARGE_LOAD_3_GPIO_CLK_ENABLE() __HAL_RCC_GPIOI_CLK_ENABLE()
#define BMS_DISCHARGE_LOAD_3_GPIO_PORT         GPIOI
#define BMS_DISCHARGE_LOAD_3_GPIO_PIN          GPIO_PIN_3

// ARD_D6

#define BMS_DISCHARGE_LOAD_4_GPIO_CLK_ENABLE() __HAL_RCC_GPIOH_CLK_ENABLE()
#define BMS_DISCHARGE_LOAD_4_GPIO_PORT         GPIOH
#define BMS_DISCHARGE_LOAD_4_GPIO_PIN          GPIO_PIN_6

// ARD_D5

#define BMS_CHARGE_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOI_CLK_ENABLE()
#define BMS_CHARGE_GPIO_PORT                   GPIOI
#define BMS_CHARGE_GPIO_PIN                    GPIO_PIN_0

// bms_log_data_to_file

static uint8_t bms_log_data_to_file(BMS_DATA* bms_data)
{
    FATFS           SDFatFs; 
    FIL             file;
    UINT            byteswritten;
    RTC_TimeTypeDef time_struct;
    RTC_DateTypeDef date_struct;
    char            sd_path[4];
    char            file_path[15];
    char            data_point_string[128];
    uint8_t         result;

    DEBUG_ASSERT(bms_data);
    
    result= BMS_RESULT_OK;

    // Make sure SD card is inserted.
    
    if ( BSP_SD_IsDetected() != SD_PRESENT ) 
        return (BMS_RESULT_ERROR);

    // Link the SD card I/O driver.
    
    memset(sd_path, 0, sizeof(sd_path));
    if ( FATFS_LinkDriver(&SD_Driver, sd_path) != FR_OK )
        return (BMS_RESULT_ERROR);

    // Register the file system object to the FatFs module.

	if ( f_mount(&SDFatFs, (const TCHAR*)sd_path, 0) != FR_OK )
    {
        result= BMS_RESULT_ERROR;
        goto end;
    }

    // Check that charge cycle data file exists.

    memset(file_path, 0, sizeof(file_path));
    snprintf(file_path, sizeof(file_path), "c%lu.txt", bms_data->bms_cycle);

    // Open data file. Create a new file if file does not exist.

    if ( f_open(&file, (const TCHAR*)file_path, FA_OPEN_ALWAYS | FA_WRITE) != FR_OK ) 
    {
        result= BMS_RESULT_ERROR;
        goto end;
    }
    
    // Append header if a new file was created.

    if ( file.fsize == 0 )
    {
        memset(data_point_string, 0, sizeof(data_point_string));
        snprintf(data_point_string, sizeof(data_point_string), "%s", "#date              #cell 1  #cell 2  #cell 3  #cell 4");
    }
    else
    {
        // Seek to end of the file.
        
        if ( f_lseek(&file, f_size(&file)) != FR_OK ) 
		{
            result= BMS_RESULT_ERROR;
            goto end;
        }

        // Get RTC time and date.

        time_struct= rtc_get_time();
        date_struct= rtc_get_date();

        // Format data string.

        memset(data_point_string, 0, sizeof(data_point_string));
        snprintf(data_point_string, sizeof(data_point_string), "\n#%d/%d/%d %02d:%02d:%02d #%.4f V #%.4f V #%.4f V #%.4f V", 
                 date_struct.Year+2000, 
                 date_struct.Month, 
                 date_struct.Date, 
                 time_struct.Hours, 
                 time_struct.Minutes, 
                 time_struct.Seconds, 
                 bms_data->cell_voltage[0],
                 bms_data->cell_voltage[1],
                 bms_data->cell_voltage[2],
                 bms_data->cell_voltage[3]);
    }

    // Write data to file.
    
    if ( f_write(&file, data_point_string, strnlen(data_point_string, sizeof(data_point_string)), &byteswritten) != FR_OK )
	{
        result= BMS_RESULT_ERROR;
    }

    end:

    // Close the data file.

    f_close(&file);

    // Unlink driver.
    
    FATFS_UnLinkDriver(sd_path);

    return (result);
}

// bms_charge_cells

static inline void bms_charge_cells(uint8_t functional_state)
{
    // Set charging relay on/off.

    HAL_GPIO_WritePin(BMS_CHARGE_GPIO_PORT, BMS_CHARGE_GPIO_PIN, functional_state == BMS_FUNCTION_STATE_ENABLE ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

// bms_discharge_cells

static inline void bms_discharge_cells(uint8_t functional_state)
{
    // Set all discharge relays on/off.

    HAL_GPIO_WritePin(BMS_DISCHARGE_LOAD_1_GPIO_PORT, BMS_DISCHARGE_LOAD_1_GPIO_PIN, functional_state == BMS_FUNCTION_STATE_ENABLE ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(BMS_DISCHARGE_LOAD_2_GPIO_PORT, BMS_DISCHARGE_LOAD_2_GPIO_PIN, functional_state == BMS_FUNCTION_STATE_ENABLE ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(BMS_DISCHARGE_LOAD_3_GPIO_PORT, BMS_DISCHARGE_LOAD_3_GPIO_PIN, functional_state == BMS_FUNCTION_STATE_ENABLE ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(BMS_DISCHARGE_LOAD_4_GPIO_PORT, BMS_DISCHARGE_LOAD_4_GPIO_PIN, functional_state == BMS_FUNCTION_STATE_ENABLE ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

// bms_init_relay_pins

static void bms_init_relay_pins(void)
{
    GPIO_InitTypeDef gpio_init_structure;

    // Configure GPIO pins that are controlling relays used for
    // charging and discharging batteries.

    gpio_init_structure.Mode=  GPIO_MODE_OUTPUT_PP;
    gpio_init_structure.Pull=  GPIO_PULLUP;
    gpio_init_structure.Speed= GPIO_SPEED_HIGH;

    // Discharge relay 1 pin initialization.

    gpio_init_structure.Pin=   BMS_DISCHARGE_LOAD_1_GPIO_PIN;

    BMS_DISCHARGE_LOAD_1_GPIO_CLK_ENABLE();
    HAL_GPIO_Init(BMS_DISCHARGE_LOAD_1_GPIO_PORT, &gpio_init_structure);

    // Discharge relay 2 pin initialization.

    gpio_init_structure.Pin=   BMS_DISCHARGE_LOAD_2_GPIO_PIN;

    BMS_DISCHARGE_LOAD_2_GPIO_CLK_ENABLE();
    HAL_GPIO_Init(BMS_DISCHARGE_LOAD_2_GPIO_PORT, &gpio_init_structure);

    // Discharge relay 3 pin initialization.

    gpio_init_structure.Pin=   BMS_DISCHARGE_LOAD_3_GPIO_PIN;

    BMS_DISCHARGE_LOAD_3_GPIO_CLK_ENABLE();
    HAL_GPIO_Init(BMS_DISCHARGE_LOAD_3_GPIO_PORT, &gpio_init_structure);

    // Discharge relay 4 pin initialization.

    gpio_init_structure.Pin=   BMS_DISCHARGE_LOAD_4_GPIO_PIN;

    BMS_DISCHARGE_LOAD_4_GPIO_CLK_ENABLE();
    HAL_GPIO_Init(BMS_DISCHARGE_LOAD_4_GPIO_PORT, &gpio_init_structure);

    bms_discharge_cells(BMS_FUNCTION_STATE_DISABLE);

    // Charging relay pin initialization.

    gpio_init_structure.Pin=   BMS_CHARGE_GPIO_PIN;

    BMS_CHARGE_GPIO_CLK_ENABLE();
    HAL_GPIO_Init(BMS_CHARGE_GPIO_PORT, &gpio_init_structure);

    bms_charge_cells(BMS_FUNCTION_STATE_DISABLE);
}

// bms_hardware

static void bms_hardware_config(cell_asic* bms_ic)
{
    DEBUG_ASSERT(bms_ic);

    // Initialize SPI bus and GPIO's used to communicate with DC2259A board.

    DC2259A_Init();
    
    LTC681x_init_cfg(1, bms_ic);
    LTC6811_reset_crc_count(1, bms_ic);
    LTC6811_init_reg_limits(1, bms_ic);

    // Write configuration register.

    wakeup_sleep(1);
    LTC6811_wrcfg(1, bms_ic);
}

// bms_is_charging_complete
// Returns 1 if cells voltage is close or above max limit.

static uint8_t bms_is_charging_complete(BMS_DATA* bms_data)
{
    if ( !bms_data )
        return (BMS_RESULT_ERROR);
    
    if ( bms_data->cell_voltage[0] >= BMS_CELL_VOLTAGE_MAX &&
         bms_data->cell_voltage[1] >= BMS_CELL_VOLTAGE_MAX &&
         bms_data->cell_voltage[2] >= BMS_CELL_VOLTAGE_MAX &&
         bms_data->cell_voltage[3] >= BMS_CELL_VOLTAGE_MAX )
    {
        return (BMS_RESULT_COMPLETE);
    }
    
    return (BMS_RESULT_INCOMPLETE);
}

// bms_is_discharging_complete

static uint8_t bms_is_discharging_complete(BMS_DATA* bms_data,
                                           uint8_t   cell)
{
    uint8_t i;

    if ( !bms_data || cell < 0 || cell > BMS_ALL_CELLS )
        return (BMS_RESULT_ERROR);
    
    // Check all cells.

    if ( cell == BMS_ALL_CELLS )
    {
        for ( i= 0; i < BMS_CELL_COUNT; i++ )
        {
            if ( (bms_data->cell_voltage[i] > BMS_CELL_VOLTAGE_MIN) )
                return (BMS_RESULT_INCOMPLETE);
        }

        return (BMS_RESULT_COMPLETE);
    }

    // Check spesific cell.

    else if ( (bms_data->cell_voltage[cell] <= BMS_CELL_VOLTAGE_MIN) )
    {
        return (BMS_RESULT_COMPLETE);
    }

    return (BMS_RESULT_INCOMPLETE);
}

// bms_read_data

static uint8_t bms_read_data(BMS_DATA*  bms_data,
                             cell_asic* bms_ic)
{
    static float cell_v_1;
    static float cell_v_2;
    static float cell_v_3;
    static float cell_v_4;
    float        mult;
    uint8_t      channels;
    int8_t       error;

    if ( !bms_ic || !bms_data )
        return (BMS_RESULT_ERROR);

    // Simulate data coming from DC2259A board.

    if ( bms_data->bms_simulate_data && (bms_data->bms_state == BMS_STATE_CHARGING || bms_data->bms_state == BMS_STATE_DISCHARGING) )
    {
        mult= bms_data->bms_state == BMS_STATE_DISCHARGING ? 1.0f : -1.0f;

        cell_v_1= MIN(MAX((cell_v_1-= (mult * 0.010f)), BMS_CELL_VOLTAGE_MAX), BMS_CELL_VOLTAGE_MIN);
        cell_v_2= MIN(MAX((cell_v_2-= (mult * 0.014f)), BMS_CELL_VOLTAGE_MAX), BMS_CELL_VOLTAGE_MIN);
        cell_v_3= MIN(MAX((cell_v_3-= (mult * 0.012f)), BMS_CELL_VOLTAGE_MAX), BMS_CELL_VOLTAGE_MIN);
        cell_v_4= MIN(MAX((cell_v_4-= (mult * 0.018f)), BMS_CELL_VOLTAGE_MAX), BMS_CELL_VOLTAGE_MIN);

        bms_data->cell_voltage[0]= cell_v_1;
        bms_data->cell_voltage[1]= cell_v_2;
        bms_data->cell_voltage[2]= cell_v_3;
        bms_data->cell_voltage[3]= cell_v_4;
    }

    if ( bms_data->bms_simulate_data )
        return (BMS_RESULT_OK);

    wakeup_idle(1);
    LTC6811_adcv(MD_7KHZ_3KHZ, DCP_DISABLED, CELL_CH_ALL);
    LTC6811_pollAdc();
    wakeup_idle(1);

    // Read cell voltage registers.

    error= LTC6811_rdcv(0, 1, bms_ic);
    if ( error == -1 )
    {
#if defined(WITH_DEBUG_LOG)
        DEBUG_LOG("A PEC error was detected while reading LTC6811 cell voltage register");
#endif
        return (BMS_RESULT_OK);
        //return (BMS_RESULT_ERROR);
    }

    // Move data to BMS_DATA struct.

    channels= bms_ic->ic_reg.cell_channels;
    bms_data->cell_voltage[0]= (bms_ic->cells.c_codes[channels-1]*0.0001);
    bms_data->cell_voltage[1]= (bms_ic->cells.c_codes[channels-2]*0.0001);
    bms_data->cell_voltage[2]= (bms_ic->cells.c_codes[channels-3]*0.0001);
    bms_data->cell_voltage[3]= (bms_ic->cells.c_codes[channels-4]*0.0001);

    return (BMS_RESULT_OK);
}

// bms_error_handler

static void bms_error_handler(BMS_DATA* bms_data)
{
    // Called when error occurs with charging/discharging.
    // Shutdown charging and discharging relays.
    
    bms_discharge_cells(BMS_FUNCTION_STATE_DISABLE);
    bms_charge_cells(BMS_FUNCTION_STATE_DISABLE);

    if ( bms_data )
        bms_data->bms_state= BMS_STATE_STOPPED;
}

// bms_set_simulation_on

uint8_t bms_set_simulation_on(BMS_DATA* bms_data,
                              uint8_t   functional_state)
{
    if ( !bms_data )
        return (BMS_RESULT_ERROR);

    bms_data->bms_simulate_data= functional_state;

    return (BMS_RESULT_OK);
}

// bms_start_test_cycle

uint8_t bms_start_test_cycle(BMS_DATA* bms_data)
{
    uint8_t result;

    if ( !bms_data )
    {
        bms_error_handler(0);
        return (BMS_RESULT_ERROR);
    }

    // Check charge level.

    result= bms_is_charging_complete(bms_data);
    if ( result == BMS_RESULT_ERROR )
    {
        bms_data->bms_state= BMS_STATE_STOPPED;
        bms_error_handler(bms_data);
        return (BMS_RESULT_ERROR);
    }

    // Start cycle by charging all battery cells.

    else if ( result == BMS_RESULT_INCOMPLETE )
    {
        bms_data->bms_state= BMS_STATE_CHARGING;
        bms_charge_cells(BMS_FUNCTION_STATE_ENABLE);
        bms_discharge_cells(BMS_FUNCTION_STATE_DISABLE);
    }
    
    // If batteries are full, then proceed by discharging them.

    else if ( result == BMS_RESULT_COMPLETE )
    {
        bms_data->bms_state= BMS_STATE_DISCHARGING;
        bms_discharge_cells(BMS_FUNCTION_STATE_DISABLE);
        bms_charge_cells(BMS_FUNCTION_STATE_ENABLE);
    }

    return (BMS_RESULT_OK);
}

// bms_stop_test_cycle

uint8_t bms_stop_test_cycle(BMS_DATA* bms_data)
{
    if ( !bms_data )
    {
        bms_error_handler(0);
        return (BMS_RESULT_ERROR);
    }

    bms_data->bms_state= BMS_STATE_STOPPED;
    bms_discharge_cells(BMS_FUNCTION_STATE_DISABLE);
    bms_charge_cells(BMS_FUNCTION_STATE_DISABLE);

    return (BMS_RESULT_OK);
}

// bms_run

uint8_t bms_run(BMS_DATA*  bms_data,
                cell_asic* bms_ic)
{
    if ( !bms_ic || !bms_data )
    {
        bms_error_handler(0);
        return (BMS_RESULT_ERROR);
    }

    // Read battery cell voltage registers.

    if ( bms_read_data(bms_data, bms_ic) == BMS_RESULT_ERROR )
    {
        bms_error_handler(bms_data);
        return (BMS_RESULT_ERROR);
    }

    // Log cell voltage.

    if ( (bms_data->bms_state == BMS_STATE_CHARGING || bms_data->bms_state == BMS_STATE_DISCHARGING) && BSP_SD_IsDetected() == SD_PRESENT )
    {
        if ( bms_log_data_to_file(bms_data) == BMS_RESULT_ERROR )
        {
            bms_error_handler(bms_data);
            return (BMS_RESULT_ERROR);
        }
    }

    // Reset state. Initialize relay pins and peripherals
    // used to communicate with DC2259A board.

    if ( bms_data->bms_state == BMS_STATE_RESET )
    {
        bms_hardware_config(bms_ic);
        bms_init_relay_pins();
        bms_data->bms_state= BMS_STATE_STOPPED;
    }
            
    // Charging. Check if charging is complete.

    else if ( bms_data->bms_state == BMS_STATE_CHARGING && bms_is_charging_complete(bms_data) == BMS_RESULT_COMPLETE )
    {
        // Stop charging batteries.
                 
        bms_charge_cells(BMS_FUNCTION_STATE_DISABLE);

        // Start discharging.

        bms_data->bms_state= BMS_STATE_DISCHARGING;
        bms_discharge_cells(BMS_FUNCTION_STATE_ENABLE);
    }

    // Discharging. Check if discharging is complete.

    else if ( bms_data->bms_state == BMS_STATE_DISCHARGING && bms_is_discharging_complete(bms_data, BMS_ALL_CELLS) == BMS_RESULT_COMPLETE )
    {
        // End of the test cycle. Stop discharging batteries.

        bms_discharge_cells(BMS_FUNCTION_STATE_DISABLE);

        // Incrase cycle count.

        bms_data->bms_cycle+= 1;

        // Stop cycle.

        bms_data->bms_state= BMS_STATE_STOPPED;
    }

    return (BMS_RESULT_OK);
}
