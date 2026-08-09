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
    const struct oxy_input_event_t *event = oxy_GetInputEvent();

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
   
    widget->state.selected = event->hovered == widget || oxy_IsWidgetFocused(widget);
    if (event->primary_pressed && event->hovered == widget)
    {
        oxy_FocusWidget(widget);
        oxy_CaptureWidget(widget);
    }
    if (!widget->state.selected && event->captured != widget)
        return;

    CURSOR_STATE_ID = CURSOR_STATE_CLICK;

    // If there is anything Hover Section run hover function
    if (widget->cursor_info.hover != NULL)
        widget->cursor_info.hover(widget->cursor_info.hover_arg);

    if (event->primary_released && event->captured == widget && event->hovered == widget)
    {
        if (widget->cursor_info.right_click)
            widget->cursor_info.right_click(widget->cursor_info.right_arg);
    }
    if (event->secondary_pressed && event->hovered == widget)
    {
        if (widget->cursor_info.left_click)
            widget->cursor_info.left_click(widget->cursor_info.left_arg);
    }
    widget->state.clicked = event->primary_down && event->captured == widget;
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
    oxy_OutlinedRoundRectangle(widget->position.x, widget->position.y,
                               widget->size.width, widget->size.height,
                               rect_color, widget->color.color_a);

    oxy_SetTextColor(text_fg, text_bg);

    // Render the children of the widget if it has any
    if (widget->child)
        oxy_RenderStack(widget->child);

    if (button->label)
    {
		button->label->widget.state.selected = widget->state.selected;
		button->label->widget.state.clicked = widget->state.clicked;
        button->label->widget.render(&button->label->widget);
    }

    if (button->sprite)
    {
        button->sprite->widget.render(&button->sprite->widget);
    }
}