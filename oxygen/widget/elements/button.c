#include "button.h"

#include "../util.h"
#include "../loop.h"
#include "../color.h"
#include "../../oxy_cursor.h"
#include "../../oxy_gfx.h"

#include <graphx.h>
#include <keypadc.h>

void oxy_UpdateButton(struct oxy_widget_t *widget)
{
    struct oxy_button_t *button = (struct oxy_button_t *)widget;

     widget->state.selected = false;

    // If the widget isn't visible then return right away
    if (!widget->state.visible || !widget->state.clickable)
        return;

    // Update size and widget
    if (button->label)
    {
        if (!(button->widget.size.width >= button->label->widget.size.width))
        {
            button->widget.size.width = button->label->widget.size.width;
        }

        if (!(button->widget.size.height >= button->label->widget.size.height)){
            button->widget.size.height = 9;
        }
    }

    if (button->sprite)
    {
        if (!(button->widget.size.width >= button->sprite->widget.size.width))
        {
            button->widget.size.width = button->sprite->widget.size.width;
        }

        if (!(button->widget.size.height >= button->sprite->widget.size.height))
        {
            button->widget.size.height = button->sprite->widget.size.height;
        }
    }
   
    // If the cursor is not overlapping with the widget check if it's clickable and prepare for a click
    if (CURSOR_STATE_ID != CURSOR_STATE_IDLE && !oxy_CheckCursorOverlap(widget))
        return;

    widget->state.selected = true;
    CURSOR_STATE_ID = CURSOR_STATE_CLICK;

    // If there is anything Hover Section run hover function
    if (widget->cursor_info.hover != NULL)
        widget->cursor_info.hover(widget->cursor_info.hover_arg);

    // Right Click
    if (kb_Data[6] & kb_Enter || kb_Data[1] & kb_2nd)
    {
        if (!widget->state.clicked && widget->state.selected)
        {
            widget->cursor_info.right_click(widget->cursor_info.right_arg);
            widget->state.clicked = true;
        }
    }
    else
    {
        widget->state.clicked = false;
    }

    // Left Click
    if (kb_Data[2] & kb_Alpha)
    {
        if (!widget->state.clicked && widget->state.selected)
        {
            widget->cursor_info.left_click(widget->cursor_info.left_arg);
            widget->state.clicked = true;
        }
    }
    else
    {
        widget->state.clicked = false;
    }
}

void oxy_RenderButton(struct oxy_widget_t *widget)
{
    struct oxy_button_t *button = (struct oxy_button_t *)widget;

    // Return immediately if the widget is not visible
    if (!widget->state.visible)
        return;

    // Determine the colors to use based on the widget state
    uint8_t rect_color, text_fg, text_bg;
    if (widget->state.selected)
    {
        if (widget->state.clicked)
        {
            rect_color = widget->color.clicked;
            text_fg = widget->color.text_fg_clicked;
            text_bg = widget->color.text_bg_clicked;
        }
        else
        {
            rect_color = widget->color.selected;
            text_fg = widget->color.text_fg_selected;
            text_bg = widget->color.text_bg_selected;
        }
    }
    else
    {
        rect_color = widget->color.unselected;
        text_fg = widget->color.text_fg_unselected;
        text_bg = widget->color.text_bg_unselected;
    }

    // Render the button
    oxy_OutlinedRectangle(widget->position.x, widget->position.y, widget->size.width, widget->size.height,
                          rect_color, widget->color.color_a);

    oxy_SetTextColor(text_fg, text_bg);

    // Render the children of the widget if it has any
    if (widget->child)
        oxy_RenderStack(widget->child);

    if (button->label)
    {
        button->label->widget.render(&button->label->widget);
    }

    if (button->sprite)
    {
        button->sprite->widget.render(&button->sprite->widget);
    }
}