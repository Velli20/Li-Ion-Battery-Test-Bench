
// Includes

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "cmsis_os.h"
#include "ui.h"
#include "stm32f7xx_rtc.h"
#include "log.h"

// Defines

#define UI_HEIGHT 272
#define UI_WIDTH  480

// Variables

extern LTDC_HandleTypeDef hLtdcHandler;
static osSemaphoreId      draw_semaphore;

// ui_draw_start

uint8_t ui_draw_start(void)
{
    if ( !draw_semaphore )
        return (UI_RC_ERROR);

    // Try to obtain draw semaphore.

    if ( osSemaphoreWait(draw_semaphore , 100) == osOK )
        return (UI_RC_OK);

    return (UI_RC_ERROR);
}

// ui_draw_end

static void ui_draw_end(void)
{
    // Release semaphore.

    if ( draw_semaphore )
        osSemaphoreRelease(draw_semaphore);
}

// draw_widget

void ui_draw_widget(WIDGET* widget)
{
    if ( ui_draw_start() != UI_RC_OK )
        return;

    __HAL_LTDC_DISABLE(&hLtdcHandler);

    // Draw widget background.

    if ( widget->state & UI_STATE_DRAW_ENABLED )
    {
        BSP_LCD_DrawBitmap(widget->x_pos, widget->y_pos, (widget->state & UI_STATE_TOUCH_DOWN) ? widget->background_image_pressed : widget->background_image);
    }

    // Clear widget area if drawing is disabled.

    else
    {
        // BSP_LCD_SetTextColor(0x000000);
        // BSP_LCD_FillRect(widget->x_pos, widget->y_pos, widget->width, widget->height);
    }
        
    __HAL_LTDC_ENABLE(&hLtdcHandler);

    ui_draw_end();
}

// ui_init

uint8_t ui_init(UI_HANDLE* ui_handle)
{    
    WIDGET* widget_ptr;

    if ( !ui_handle )
        return (UI_RC_ERROR);

    // Create draw semaphore.

    osSemaphoreDef(draw);
    draw_semaphore= osSemaphoreCreate(osSemaphore(draw), 1);
    if ( !draw_semaphore )
        return (UI_RC_ERROR);

    // Initialize the LCD.

    if ( BSP_LCD_Init() != LCD_OK )
        return (UI_RC_ERROR);

    // Initialize touch screen.

    if ( BSP_TS_Init(ui_handle->screen_width, ui_handle->screen_height) != TS_OK )
        return (UI_RC_ERROR);

    // Configure touch interrupt.

    if ( BSP_TS_ITConfig() != TS_OK )
        return (UI_RC_ERROR);

    // Initialize LCD layer 0 frame buffer.

    if ( ui_handle->layer_0_address )
        BSP_LCD_LayerRgb565Init(0, ui_handle->layer_0_address);

    // Initialize LCD layer 1 frame buffer.

    if ( ui_handle->layer_1_address )
        BSP_LCD_LayerDefaultInit(1, LCD_FB_START_ADDRESS);

    // Set LCD Foreground Layer.

    BSP_LCD_SelectLayer(1);

    // Set layer 1 background fully transparent.

    BSP_LCD_SetBackColor(0x000000);

    // Clear the LCD.

    BSP_LCD_Clear(0x000000);  

    // Make sure that UI initialization is called only once upon startup.

    ui_handle->touch_state= UI_STATE_TOUCH_UP;

    // Draw widgets.

    widget_ptr= ui_handle->first_widget;
    while ( widget_ptr )
    {
        ui_draw_widget(widget_ptr);
        widget_ptr= widget_ptr->next_widget;
    }

    return (UI_RC_OK);
}

// display_time

uint8_t ui_display_time(void)
{
    RTC_TimeTypeDef time_struct;
    RTC_DateTypeDef date_struct;
    char            date_time_text[50];

    if ( ui_draw_start() != UI_RC_OK )
        return (UI_RC_ERROR);

    memset(date_time_text, 0, sizeof(date_time_text));
    
    // Get RTC time and date.

    time_struct= rtc_get_time();
    date_struct= rtc_get_date();

    // Format date time string.

    snprintf(date_time_text, sizeof(date_time_text)-1, ("%d/%d/%d %02d:%02d:%02d"), 
            (date_struct.Year+2000), date_struct.Month, date_struct.Date, 
            time_struct.Hours, time_struct.Minutes, time_struct.Seconds);

    // Display string.

    BSP_LCD_SetFont(&Font12);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_DisplayStringAt(10, 5, (uint8_t*)date_time_text, RIGHT_MODE);

    ui_draw_end();

    return (UI_RC_OK);
}

// ui_display_status_bar_string

uint8_t ui_display_status_bar_string(uint8_t* buffer)
{
    if ( !buffer || !*buffer )
        return (UI_RC_ERROR);

    if ( ui_draw_start() != UI_RC_OK )
        return (UI_RC_ERROR);

    // Clear status bar string area.

    BSP_LCD_SetTextColor(0x000000);
    BSP_LCD_FillRect(0, 0, 320, 30);

    // Display string on a status bar.

    BSP_LCD_SetFont(&Font12);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_DisplayStringAt(10, 5, buffer, LEFT_MODE);

    ui_draw_end();

    return (UI_RC_OK);
}

// ui_display_cell_voltage

uint8_t ui_display_cell_voltage(float cell_1_voltage,
                                float cell_2_voltage,
                                float cell_3_voltage,
                                float cell_4_voltage)
{
    char voltage_text[12];

    if ( ui_draw_start() != UI_RC_OK )
        return (UI_RC_ERROR);

    // Clear area.

    BSP_LCD_SetTextColor(0x000000);
    BSP_LCD_FillRect(120, 45, 130, 227);

    // Set text draw properties.

    BSP_LCD_SetFont(&Font24);
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);

    // Draw voltage texts.

    if ( cell_1_voltage <= 0.0f )
        snprintf(voltage_text, sizeof(voltage_text), "-- V");
    else
        snprintf(voltage_text, sizeof(voltage_text), "%.2f V", cell_1_voltage);

    BSP_LCD_DisplayStringAt(120, 45, (uint8_t*)voltage_text, LEFT_MODE);

    if ( cell_2_voltage <= 0.0f )
        snprintf(voltage_text, sizeof(voltage_text), "-- V");
    else
        snprintf(voltage_text, sizeof(voltage_text), "%.2f V", cell_2_voltage);

    BSP_LCD_DisplayStringAt(120, 100, (uint8_t*)voltage_text, LEFT_MODE);

    if ( cell_3_voltage <= 0.0f )
        snprintf(voltage_text, sizeof(voltage_text), "-- V");
    else
        snprintf(voltage_text, sizeof(voltage_text), "%.2f V", cell_3_voltage);

    BSP_LCD_DisplayStringAt(120, 160, (uint8_t*)voltage_text, LEFT_MODE);

    if ( cell_4_voltage <= 0.0f )
        snprintf(voltage_text, sizeof(voltage_text), "-- V");
    else
        snprintf(voltage_text, sizeof(voltage_text), "%.2f V", cell_4_voltage);

    BSP_LCD_DisplayStringAt(120, 215, (uint8_t*)voltage_text, LEFT_MODE);

    ui_draw_end();

    return (UI_RC_OK);
}

// ui_display_bms_state_string

uint8_t ui_display_bms_state_string(uint8_t* state_string)
{
    if ( !state_string || !*state_string )
        return (UI_RC_ERROR);

    if ( ui_draw_start() != UI_RC_OK )
        return (UI_RC_ERROR);

    // Clear text area.

    BSP_LCD_SetTextColor(0x000000);
    BSP_LCD_FillRect(275, 70, 160, 20);

    // Set text draw properties and display string.

    BSP_LCD_SetFont(&Font16);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_DisplayStringAt(275, 70, state_string, LEFT_MODE);

    ui_draw_end();

    return (UI_RC_OK);
}

// ui_display_bms_test_cycle_count

uint8_t ui_display_bms_test_cycle_count(uint32_t count)
{
    char count_text[20];

    if ( ui_draw_start() != UI_RC_OK )
        return (UI_RC_ERROR);

    memset(count_text, 0, sizeof(count_text));
    snprintf(count_text, sizeof(count_text), "%lu", count);
    
    // Clear text area.

    BSP_LCD_SetTextColor(0x000000);
    BSP_LCD_FillRect(275, 120, 160, 20);

    // Set text draw properties and display string.

    BSP_LCD_SetFont(&Font16);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_DisplayStringAt(275, 120, (uint8_t*)count_text, LEFT_MODE);

    ui_draw_end();

    return (UI_RC_OK);
}

// ui_handle_event

uint8_t ui_handle_touch_event(UI_HANDLE* ui_handle)
{
    TS_StateTypeDef ts_state;
    WIDGET*         widget_ptr;

    if ( !ui_handle )
        return (UI_RC_ERROR);

    // Get touch point.

    if ( BSP_TS_GetState(&ts_state) != TS_OK )
        return (UI_RC_ERROR);

    // Touch down event.

    if ( ts_state.touchDetected && (ui_handle->touch_state == UI_STATE_TOUCH_UP) )
    {
        UI_SET_TOUCH_STATE(ui_handle, UI_STATE_TOUCH_DOWN);
        UI_STORE_TOUCH_POINT(ui_handle, ts_state.touchX[0], ts_state.touchY[0]);
    }
    
    // Touch up event.

    else if ( !ts_state.touchDetected && (ui_handle->touch_state == UI_STATE_TOUCH_DOWN) )
    {
        UI_SET_TOUCH_STATE(ui_handle, UI_STATE_TOUCH_UP);
    }

    // No change in touch state.

    else
    {
        return (UI_RC_OK);
    }

    // Redraw widgets if touch state has been changed.

    widget_ptr= ui_handle->first_widget;
    while ( widget_ptr )
    {
        if ( ts_state.touchDetected && 
             UI_IS_WIDGET_TOUCHED(widget_ptr, ts_state.touchX[0], ts_state.touchY[0]) )
        {
            UI_SET_WIDGET_TOUCH_STATE(widget_ptr, UI_STATE_TOUCH_DOWN);
            ui_draw_widget(widget_ptr);
        }
        else if ( !ts_state.touchDetected && 
                  !UI_IS_WIDGET_TOUCHED(widget_ptr, ts_state.touchX[0], ts_state.touchY[0]) && 
                  widget_ptr->state & UI_STATE_TOUCH_DOWN && 
                  widget_ptr->state & UI_STATE_DRAW_ENABLED )
        {
            UI_SET_WIDGET_TOUCH_STATE(widget_ptr, UI_STATE_TOUCH_UP);
            ui_draw_widget(widget_ptr);

            // Notify widget callback that click event has occured.

            if ( ui_handle->callback )
                ui_handle->callback(ui_handle, widget_ptr, UI_EVENT_CLICK);
        }
        
        widget_ptr= widget_ptr->next_widget;
    }

    return (UI_RC_OK);
}
