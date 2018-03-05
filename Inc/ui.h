#ifndef __UI_H
#define __UI_H

// Includes

#include "stm32f7xx_hal.h"
#include "stm32746g_discovery_lcd.h"
#include "stm32746g_discovery_ts.h"

// Defines 

#define UI_TOUCH_STATE_CLEAR_MASK        0x0F
#define UI_DRAW_STATE_CLEAR_MASK         0xF0

#define UI_STATE_TOUCH_UP                0x00
#define UI_STATE_TOUCH_MOVE              0x01
#define UI_STATE_TOUCH_DOWN              0x02
#define UI_STATE_DRAW_ENABLED            0x10
#define UI_STATE_DRAW_DISABLED           0x20

#define UI_EVENT_CLICK                   0x01

#define UI_RC_OK                         0x0
#define UI_RC_ERROR                      0x1

// Macros

#define UI_IS_WIDGET_TOUCHED(WIDGET, TOUCH_X, TOUCH_Y)    \
(                                                         \
    TOUCH_X > WIDGET->x_pos                   &&          \
    TOUCH_Y > WIDGET->y_pos                   &&          \
    TOUCH_X < (WIDGET->x_pos + WIDGET->width) &&          \
    TOUCH_Y < (WIDGET->y_pos + WIDGET->height)            \
)

#define UI_STORE_TOUCH_POINT(UI_HANDLE, TOUCH_X, TOUCH_Y) \
{                                                         \
    UI_HANDLE->touch_x= TOUCH_X;                          \
    UI_HANDLE->touch_y= TOUCH_Y;                          \
}

#define UI_SET_WIDGET_DRAW_STATE(WIDGET, NEW_STATE) ((WIDGET->state)= (((WIDGET->state) & (~(UI_DRAW_STATE_CLEAR_MASK)))  | (NEW_STATE)))
#define UI_SET_WIDGET_TOUCH_STATE(WIDGET, NEW_STATE)((WIDGET->state)= (((WIDGET->state) & (~(UI_TOUCH_STATE_CLEAR_MASK))) | (NEW_STATE)))
#define UI_SET_TOUCH_STATE(UI_HANDLE, STATE)(UI_HANDLE->touch_state= STATE)

#define UI_INVALIDATE_WIDGET(WIDGET)(ui_draw_widget(WIDGET))

// Forward declarations

typedef struct widget_struct WIDGET;
typedef struct ui_struct     UI_HANDLE;

// Widget callback

typedef void (*WIDGET_CALLBACK)(UI_HANDLE* ui_handle, WIDGET* widget, uint8_t event);

// WIDGET struct

typedef struct widget_struct
{
    uint8_t* background_image_pressed;
    uint8_t* background_image;
    uint8_t  state;

    uint16_t width;
    uint16_t height;
    uint16_t y_pos;
    uint16_t x_pos;
    uint16_t id;

    WIDGET*  next_widget;
}
WIDGET;

// UI struct

typedef struct ui_struct
{
    uint32_t          layer_0_address;
    uint32_t          layer_1_address;
    uint32_t          screen_width;
    uint32_t          screen_height;

    volatile uint16_t touch_x;
    volatile uint16_t touch_y;
    volatile uint8_t  touch_state;

    WIDGET*           first_widget;
    WIDGET*           last_widget;

    WIDGET_CALLBACK   callback;
}
UI_HANDLE;

// ui_init

uint8_t ui_init(UI_HANDLE* ui_handle);

// ui_draw_widget

void ui_draw_widget(WIDGET* widget);

// display_time

uint8_t ui_display_time(void);

// ui_display_status_bar_string

uint8_t ui_display_status_bar_string(uint8_t* buffer);

// ui_display_cell_voltage

uint8_t ui_display_cell_voltage(float cell_1_voltage,
                                float cell_2_voltage,
                                float cell_3_voltage,
                                float cell_4_voltage);

// ui_display_bms_state_string

uint8_t ui_display_bms_state_string(uint8_t* state_string);

// ui_display_bms_test_cycle_count

uint8_t ui_display_bms_test_cycle_count(uint32_t count);

// ui_handle_touch_event

uint8_t ui_handle_touch_event(UI_HANDLE* ui_handle);

// ui_add_widget

uint8_t ui_add_widget(UI_HANDLE* ui_handle,
                      WIDGET*    widget);

// ui_find_widget_with_id

WIDGET* ui_find_widget_with_id(UI_HANDLE* ui_handle,
                               uint16_t   widget_id);

// ui_free_widget_list

uint8_t ui_free_widget_list(UI_HANDLE* ui_handle);

#endif