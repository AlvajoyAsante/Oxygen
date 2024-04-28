#include "rectangle.h"

#include "../util.h"
#include "../loop.h"
#include "../color.h"
#include "../../oxy_cursor.h"
#include "../../oxy_gfx.h"

#include <graphx.h>
#include <keypadc.h>

void oxy_UpdateRectangle(struct oxy_widget_t *widget)
{
    // struct oxy_rectangle_t *rectangle = (struct oxy_rectangle_t *)widget;

    if (!widget->state.visible)
        return;
    
    if (CURSOR_STATE_ID == CURSOR_STATE_IDLE && oxy_CheckCursorOverlap(widget))
    {
        if (widget->state.clickable)
        {
            widget->state.selected = true;
            CURSOR_STATE_ID = CURSOR_STATE_CLICK;
        }
        else
        {
            widget->state.selected = false;
        }
    }
    else
    {
        widget->state.selected = false;
    }

    // Check if the widget was selected if not return. because there is not reason to check if user click if the widget has not been selected.
    if (!widget->state.selected)
        return;

    if (kb_Data[6] & kb_Enter || kb_Data[1] & kb_2nd) // Left Click
    {

        if (!widget->state.clicked && widget->state.selected)
        {
            if (widget->cursor_info.left_click != NULL)
                widget->cursor_info.left_click(widget->cursor_info.left_arg);

            widget->state.clicked = true;
        }
    }
    else if (kb_Data[1] & kb_KeyAlpha) // Right Click
    {

        if (!widget->state.clicked && widget->state.selected)
        {
            if (widget->cursor_info.right_click != NULL)
            {
                widget->cursor_info.left_click(widget->cursor_info.left_arg);
            }

            widget->state.clicked = true;
        }
    }
    else
    {
        widget->state.clicked = false;
    }
}

void oxy_RenderRectangle(struct oxy_widget_t *widget)
{
    struct oxy_rectangle_t *rectangle = (struct oxy_rectangle_t *)widget;

    if (widget->state.visible)
    {
        // is the rectangle filled?
        if (rectangle->filled)
        {
            gfx_SetColor(widget->color.color_a);

            if (rectangle->round)
            {
                // use oxygen rectangle
                oxy_FillIndentedRectangle_All(widget->position.x, widget->position.y, widget->size.width, widget->size.height);
            }
            else
            {
                // use toolchain rectangle
                gfx_FillRectangle(widget->position.x, widget->position.y, widget->size.width, widget->size.height);
            }
        }

        if (rectangle->border)
        {
            gfx_SetColor(widget->color.color_a);

            if (rectangle->round)
            {
                oxy_FillIndentedRectangle_All(widget->position.x, widget->position.y, widget->size.width, widget->size.height);
            }
            else
            {
                gfx_Rectangle(widget->position.x, widget->position.y, widget->size.width, widget->size.height);
            }
        }
    }
}