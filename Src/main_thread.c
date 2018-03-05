
// Includes

#include "main.h"
#include "cmsis_os.h"
#include "dhcp_connect_thread.h"
#include "ethernetif.h"
#include "lwip/netif.h"
#include "lwip/tcpip.h"
#include "server.h"
#include "log.h"

// User interface includes

#if defined(WITH_BMS_USER_INTERFACE)
#include "ui.h"
#include "screen_background_480_272_rgb565.h"
#include "ui_start_button.h"
#include "ui_start_button_pressed.h"
#include "ui_stop_button.h"
#include "ui_stop_button_pressed.h"
#endif

// Variables

osMessageQId        main_thread_queue= 0;
static osMessageQId ui_thread_queue;

// BMS_STATE_STRING

#if defined(WITH_BMS_USER_INTERFACE)
static uint8_t* BMS_STATE_STRING[]=
{
    (uint8_t*)"RESET",
    (uint8_t*)"STOPPED",
    (uint8_t*)"CHARGING",
    (uint8_t*)"DISCHARGING"
};
#endif

// ui_callback

static void ui_callback(UI_HANDLE* ui_handle,
                        WIDGET*    widget,
                        uint8_t    event)
{
    osStatus result;

    DEBUG_ASSERT(widget && ui_handle);
    
    // Handle only start and stop button click events.

    if ( (widget->id != STOP_BUTTON_ID && widget->id != START_BUTTON_ID) || event != UI_EVENT_CLICK )
        return;

    // Disable clicked button.

    UI_SET_WIDGET_DRAW_STATE(widget, UI_STATE_DRAW_DISABLED);
    UI_INVALIDATE_WIDGET(widget);

    // Notify main thread.
        
    result= osMessagePut(main_thread_queue, widget->id == STOP_BUTTON_ID ? CMSIS_OS_EVENT_STOP_BUTTON_CLICKED : CMSIS_OS_EVENT_START_BUTTON_CLICKED, osWaitForever);
    DEBUG_ASSERT(result == osOK);
}

// configure_ui

#if defined(WITH_BMS_USER_INTERFACE)
static void configure_bms_ui(UI_HANDLE* ui_handle)
{
    WIDGET* widget;

    if ( !ui_handle )
        return;

    // Configure the battery monitoring station user interface.
    // Set up layer 0 frame buffer address. Use static image that locates on external flash storage.

    ui_handle->layer_0_address= (uint32_t)&screen_background_480_272_rgb565;

    // Set up layer 1. This layer is used for drawing widgets.
    // Use SDRAM for this layer.

    ui_handle->layer_1_address= LCD_FB_START_ADDRESS;
    ui_handle->screen_width=    480;
    ui_handle->screen_height=   272;
    ui_handle->first_widget=    0;
    ui_handle->last_widget=     0;
    ui_handle->callback=        ui_callback;

    // Start button widget.

    widget= calloc(1, sizeof(WIDGET));
    if ( !widget )
        return;
    
    widget->y_pos=  215;
    widget->x_pos=  375;
    widget->width=  100;
    widget->height= 50;
    widget->state=  0;
    widget->id=     START_BUTTON_ID;
    widget->background_image=         (uint8_t*)ui_start_button;
    widget->background_image_pressed= (uint8_t*)ui_start_button_pressed;

    UI_SET_WIDGET_DRAW_STATE(widget, UI_STATE_DRAW_DISABLED);

    ui_add_widget(ui_handle, widget);

    // Stop button widget.

    widget= calloc(1, sizeof(WIDGET));
    if ( !widget )
        return;

    widget->y_pos=  215;
    widget->x_pos=  375;
    widget->width=  100;
    widget->height= 50;
    widget->state=  0;
    widget->id=     STOP_BUTTON_ID;
    widget->background_image=         (uint8_t*)ui_stop_button;
    widget->background_image_pressed= (uint8_t*)ui_stop_button_pressed;

    // Disable drawing for stop button.

    UI_SET_WIDGET_DRAW_STATE(widget, UI_STATE_DRAW_DISABLED);

    ui_add_widget(ui_handle, widget);
}
#endif

// UI thread

#if defined(WITH_BMS_USER_INTERFACE) 
static void ui_thread(void const* argument)
{
    BMS_DATA*    bms_data;
    osEvent      ui_event;
    UI_HANDLE    ui_handle;
    WIDGET*      widget;
    uint8_t      ui_rc_code;

    bms_data= (BMS_DATA*)argument;
    DEBUG_ASSERT(bms_data);

    // Create message queue to communicate ui thread.

    osMessageQDef(ui_queue, CMSIS_OS_UI_THREAD_MESSAGE_QUEUE_SIZE, uint16_t);
    ui_thread_queue= osMessageCreate(osMessageQ(ui_queue), NULL);
    DEBUG_ASSERT(ui_thread_queue);

    // Set up user interface.

    configure_bms_ui(&ui_handle);

    // Initialize UI.

    DEBUG_ASSERT(ui_init(&ui_handle) == UI_RC_OK);

    for ( ;; )
    {
        // Wait for UI event for 1000 ms. 

        ui_event= osMessageGet(ui_thread_queue, 1000);
        if ( ui_event.status != osEventMessage )
            goto no_ui_events;

        // Handle UI touch event.

        if ( ui_event.value.v == CMSIS_OS_EVENT_TOUCH )
        {
            DEBUG_ASSERT(ui_handle_touch_event(&ui_handle) == UI_RC_OK);
            continue;
        }

        // BMS state has been changed. Update UI widgets to represent new state.

        else if ( ui_event.value.v == CMSIS_OS_EVENT_BMS_STATE_CHANGED )
        {
            // Update start button status.

            widget= ui_find_widget_with_id(&ui_handle, START_BUTTON_ID);
            DEBUG_ASSERT(widget);

            UI_SET_WIDGET_DRAW_STATE(widget, bms_data->bms_state == BMS_STATE_STOPPED ? UI_STATE_DRAW_ENABLED : UI_STATE_DRAW_DISABLED);
            UI_INVALIDATE_WIDGET(widget);

            // Update stop button status.
            
            widget= ui_find_widget_with_id(&ui_handle, STOP_BUTTON_ID);
            DEBUG_ASSERT(widget);

            UI_SET_WIDGET_DRAW_STATE(widget, (bms_data->bms_state == BMS_STATE_DISCHARGING || bms_data->bms_state == BMS_STATE_CHARGING)? UI_STATE_DRAW_ENABLED : UI_STATE_DRAW_DISABLED);
            UI_INVALIDATE_WIDGET(widget);

            // Display new state.

            ui_rc_code= ui_display_bms_state_string(BMS_STATE_STRING[bms_data->bms_state]);
            DEBUG_ASSERT(ui_rc_code == UI_RC_OK);

            // Display cycle count.

            ui_rc_code= ui_display_bms_test_cycle_count(bms_data->bms_cycle);
            DEBUG_ASSERT(ui_rc_code == UI_RC_OK);

            continue;
        }

        no_ui_events:

        // Display time.

        ui_display_time();

        // Display cell voltage.

        ui_display_cell_voltage(bms_data->cell_voltage[0],
                                bms_data->cell_voltage[1],
                                bms_data->cell_voltage[2],
                                bms_data->cell_voltage[3]);
    }
    
    // Delete UI message queue.

    DEBUG_ASSERT(osMessageDelete(ui_thread_queue) == osOK);

    // Free widget resources.

    DEBUG_ASSERT(ui_free_widget_list(&ui_handle) == UI_RC_OK);

    // Terminate UI thread.

    osThreadTerminate(NULL);
}
#endif


// main_thread

void main_thread(void const* argument)
{   
    osEvent      event;
    struct netif gnetif;
    cell_asic    bms_ic;
    BMS_DATA     bms_data;
    uint8_t      bms_previous_state;

    // Create message queue to communicate with main thread.

    osMessageQDef(main_queue, CMSIS_OS_MAIN_THREAD_MESSAGE_QUEUE_SIZE, uint16_t);
    main_thread_queue= osMessageCreate(osMessageQ(main_queue), NULL);
    DEBUG_ASSERT(main_thread_queue);

    // Initialize UI thread.
    
#if defined(WITH_BMS_USER_INTERFACE)
    osThreadDef(UI, ui_thread, osPriorityNormal, 0, configMINIMAL_STACK_SIZE* 10);
    osThreadCreate(osThread(UI), &bms_data);
#endif
    
    // Display connection status.

#if defined(WITH_BMS_USER_INTERFACE) 
    ui_display_status_bar_string((uint8_t*)"Initializing network interface...");
#endif

#if defined(WITH_DEBUG_LOG)
    DEBUG_LOG("Initializing network interface...");
#endif

    // Create tcp_ip stack thread.

    tcpip_init(NULL, NULL);

    // Initialize webserver.

    http_server_netconn_init(&gnetif, &bms_data);

    // Start DHCP Client.

    osThreadDef(DHCP, dhcp_connect_thread, osPriorityNormal, 0, configMINIMAL_STACK_SIZE* 4);
    osThreadCreate(osThread(DHCP), &gnetif);

    bms_data.bms_cycle=         0;
    bms_data.bms_state=         BMS_STATE_RESET;
    bms_data.bms_simulate_data= BMS_FUNCTION_STATE_ENABLE;
    bms_data.cell_voltage[0]=   BMS_CELL_VOLTAGE_MIN;
    bms_data.cell_voltage[1]=   BMS_CELL_VOLTAGE_MIN;
    bms_data.cell_voltage[2]=   BMS_CELL_VOLTAGE_MIN;
    bms_data.cell_voltage[3]=   BMS_CELL_VOLTAGE_MIN;

 #if defined(WITH_BMS_USER_INTERFACE)
    osMessagePut(ui_thread_queue, CMSIS_OS_EVENT_BMS_STATE_CHANGED, osWaitForever);
    bms_previous_state= bms_data.bms_state;
#endif

    for ( ;; )
    {
        // Wait for event and sleep max. for 1000 ms.

        event= osMessageGet(main_thread_queue, 1000);
        if ( event.status == osEventMessage )
        {
            // Start battery stack test cycle.

            if ( event.value.v == CMSIS_OS_EVENT_START_BUTTON_CLICKED )
                DEBUG_ASSERT(bms_start_test_cycle(&bms_data) == BMS_RESULT_OK);

            // Abort test cycle.

            else if ( event.value.v == CMSIS_OS_EVENT_STOP_BUTTON_CLICKED )
                DEBUG_ASSERT(bms_stop_test_cycle(&bms_data) == BMS_RESULT_OK);

            // Set bms simulation on.

            else if ( event.value.v == CMSIS_OS_EVENT_BMS_DATA_SIMULATION_ON )
                DEBUG_ASSERT(bms_set_simulation_on(&bms_data, BMS_FUNCTION_STATE_ENABLE) == BMS_RESULT_OK);

            // Set bms simulation off.

            else if ( event.value.v == CMSIS_OS_EVENT_BMS_DATA_SIMULATION_OFF )
                DEBUG_ASSERT(bms_set_simulation_on(&bms_data, BMS_FUNCTION_STATE_DISABLE) == BMS_RESULT_OK);
        }

        // Run BMS.

        bms_run(&bms_data, &bms_ic);

        // Notify http client.

        http_server_send_event_to_client(1);

        // Signal UI thread if bms state has been changed.

 #if defined(WITH_BMS_USER_INTERFACE)       
        if ( bms_data.bms_state != bms_previous_state )
        {
            bms_previous_state= bms_data.bms_state;
            osMessagePut(ui_thread_queue, CMSIS_OS_EVENT_BMS_STATE_CHANGED, osWaitForever);
        }
#endif
    }

    // Delete message queue.

    DEBUG_ASSERT(osMessageDelete(main_thread_queue) == osOK);

    // Stop main thread.

    osThreadTerminate(NULL);
}

// HAL_GPIO_EXTI_Callback

#if defined(WITH_BMS_USER_INTERFACE)
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    // Wait for touch interrupt.

    if ( GPIO_Pin == TS_INT_PIN && ui_thread_queue )
        osMessagePut(ui_thread_queue, CMSIS_OS_EVENT_TOUCH, osWaitForever);
}
#endif
