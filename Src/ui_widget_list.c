
// Includes

#include "ui.h"
#include "log.h"

// ui_add_widget

uint8_t ui_add_widget(UI_HANDLE* ui_handle,
                      WIDGET*    widget)
{
    if ( !ui_handle || !widget )
        return (UI_RC_ERROR);

    widget->next_widget= 0;

    // First widget pointer in list.

    if ( !ui_handle->first_widget )
    {
        ui_handle->first_widget= widget;
        ui_handle->last_widget=  widget;
    }

    // Link new widget pointer to last widget pointer.
    else
    {
        ui_handle->last_widget->next_widget= widget;
        ui_handle->last_widget=              widget;
    }

    return (UI_RC_OK);
}

// ui_find_widget_with_id

WIDGET* ui_find_widget_with_id(UI_HANDLE* ui_handle,
                               uint16_t   widget_id)
{
    WIDGET* widget_ptr;

    DEBUG_ASSERT(ui_handle);

    widget_ptr= ui_handle->first_widget;
    while ( widget_ptr )
    {
        if ( widget_ptr && widget_ptr->id == widget_id )
            return (widget_ptr);

        widget_ptr= widget_ptr->next_widget;
    }

    return(0);
}
