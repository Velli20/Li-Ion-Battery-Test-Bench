#ifndef __MAIN_H
#define __MAIN_H

// Includes

#include "stm32f7xx_hal.h"
#include "stm32746g_discovery.h"
#include "stm32746g_discovery_qspi.h"

// DC2259A includes

#include "LTC681x.h"
#include "LTC6811.h"
#include "bms_hardware.h"

// Defines

#define BMS_FUNCTION_STATE_DISABLE ((int8_t)0)
#define BMS_FUNCTION_STATE_ENABLE  ((int8_t)1)

#define BMS_RESULT_OK              0x00
#define BMS_RESULT_ERROR           0x01
#define BMS_RESULT_COMPLETE        0x02
#define BMS_RESULT_INCOMPLETE      0x03

#define BMS_CELL_VOLTAGE_MIN       ((float)3.7f)
#define BMS_CELL_VOLTAGE_MAX       ((float)4.2f)

#define BMS_STATE_RESET            0x00
#define BMS_STATE_STOPPED          0x01
#define BMS_STATE_CHARGING         0x02
#define BMS_STATE_DISCHARGING      0x03

#define BMS_CELL_COUNT             ((uint8_t)4)
#define BMS_CELL_1                 ((uint8_t)0)
#define BMS_CELL_2                 ((uint8_t)1)
#define BMS_CELL_3                 ((uint8_t)2)
#define BMS_CELL_4                 ((uint8_t)3)
#define BMS_ALL_CELLS              ((uint8_t)4)

// Defines

#define CMSIS_OS_UI_THREAD_MESSAGE_QUEUE_SIZE   5
#define CMSIS_OS_MAIN_THREAD_MESSAGE_QUEUE_SIZE 20

#define CMSIS_OS_EVENT_TOUCH                    0x1
#define CMSIS_OS_EVENT_START_BUTTON_CLICKED     0x2
#define CMSIS_OS_EVENT_STOP_BUTTON_CLICKED      0x3
#define CMSIS_OS_EVENT_BMS_STATE_CHANGED        0x4

// BMS_DATA struct

typedef struct bms_data
{
    float    cell_voltage[4];

    // Timestamp to indicate when data  was updated.

    time_t   timestamp;
    uint8_t  bms_state;
    uint32_t bms_cycle;
}
BMS_DATA;

// main_thread

void main_thread(void const* argument);

// bms_run

uint8_t bms_run(BMS_DATA*  bms_data,
                cell_asic* bms_ic);

// bms_start_test_cycle

uint8_t bms_start_test_cycle(BMS_DATA* bms_data);

// bms_stop_test_cycle

uint8_t bms_stop_test_cycle(BMS_DATA* bms_data);

// HAL_GPIO_EXTI_Callback

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

#endif
