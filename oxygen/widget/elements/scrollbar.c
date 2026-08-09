#include "scrollbar.h"
#include "../util.h"
#include "../loop.h"
#include "../../oxy_cursor.h"
#include "../../oxy_gfx.h"
#include <tice.h>
#include <graphx.h>
#include <keypadc.h>

void oxy_UpdateScrollbar(struct oxy_widget_t *widget)
{
    struct oxy_scrollbar_t *scrollbar = (struct oxy_scrollbar_t *)widget;
    const struct oxy_input_event_t *event = oxy_GetInputEvent();

    if (!widget->state.visible)
        return;

    if (event->primary_pressed && oxy_CheckCursorOverlap(widget))
    {
        oxy_FocusWidget(widget);
        oxy_CaptureWidget(widget);
    }
    widget->state.selected = oxy_CheckCursorOverlap(widget) || oxy_IsWidgetFocused(widget);
    if (scrollbar->max_value <= scrollbar->min_value)
        return;

    int old_value = scrollbar->current_value;
    if (event->primary_down && event->captured == widget)
    {
        int length = scrollbar->alignment == SCROLLBAR_VERTICAL ? widget->size.height : widget->size.width;
        int position = scrollbar->alignment == SCROLLBAR_VERTICAL
                           ? CURSOR_Y_POS - widget->position.y
                           : CURSOR_X_POS - widget->position.x;
        if (position < 0)
            position = 0;
        if (position > length)
            position = length;
        scrollbar->current_value = scrollbar->min_value +
                                   (scrollbar->max_value - scrollbar->min_value) * position / length;
    }
    else if (oxy_IsWidgetFocused(widget) && scrollbar->alignment == SCROLLBAR_VERTICAL)
    {
        if (event->scan_code == sk_Up)
            scrollbar->current_value--;
        if (event->scan_code == sk_Down)
            scrollbar->current_value++;
    }
    else if (oxy_IsWidgetFocused(widget))
    {
        if (event->scan_code == sk_Left)
            scrollbar->current_value--;
        if (event->scan_code == sk_Right)
            scrollbar->current_value++;
    }

    if (scrollbar->current_value < scrollbar->min_value)
        scrollbar->current_value = scrollbar->min_value;
    if (scrollbar->current_value > scrollbar->max_value)
        scrollbar->current_value = scrollbar->max_value;
    if (old_value != scrollbar->current_value)
        widget->state.redraw = true;
}

void oxy_RenderScrollbar(struct oxy_widget_t *widget)
{
    struct oxy_scrollbar_t *scrollbar = (struct oxy_scrollbar_t *)widget;

    if (!widget->state.visible)
        return;

    oxy_OutlinedRoundRectangle(widget->position.x, widget->position.y,
                               widget->size.width, widget->size.height,
                               widget->color.unselected, widget->color.color_a);

    int range = scrollbar->max_value - scrollbar->min_value;
    int offset = range > 0 ? scrollbar->current_value - scrollbar->min_value : 0;
    gfx_SetColor(widget->state.selected ? widget->color.selected : widget->color.color_b);
    if (scrollbar->alignment == SCROLLBAR_VERTICAL)
    {
        int thumb_height = widget->size.height < 8 ? widget->size.height : 8;
        int travel = widget->size.height - thumb_height;
        oxy_FillRoundRectangle(widget->position.x + 1,
                       widget->position.y + 1 + (range > 0 ? travel * offset / range : 0),
                       widget->size.width - 2,
                       thumb_height > 2 ? thumb_height - 2 : thumb_height, 0);
    }
    else
    {
        int thumb_width = widget->size.width < 8 ? widget->size.width : 8;
        int travel = widget->size.width - thumb_width;
        oxy_FillRoundRectangle(widget->position.x + 1 + (range > 0 ? travel * offset / range : 0),
                       widget->position.y + 1,
                       thumb_width > 2 ? thumb_width - 2 : thumb_width,
                       widget->size.height - 2, 0);
    }

    widget->state.redraw = false;
}