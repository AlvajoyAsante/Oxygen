#include "slider.h"
#include "scrollbar.h"
#include "../color.h"
#include "../util.h"
#include "../loop.h"
#include "../../oxy_cursor.h"
#include "../../oxy_gfx.h"

#include <keypadc.h>
#include <graphx.h>

void oxy_UpdateSlider(struct oxy_widget_t *widget)
{
    struct oxy_slider_t *slider = (struct oxy_slider_t *)widget;
    const struct oxy_input_event_t *event = oxy_GetInputEvent();

    if (!widget->state.visible)
        return;

    if (event->primary_pressed && oxy_CheckCursorOverlap(widget))
    {
        oxy_FocusWidget(widget);
        oxy_CaptureWidget(widget);
    }
    widget->state.selected = oxy_CheckCursorOverlap(widget) || oxy_IsWidgetFocused(widget);
    if (slider->max <= slider->min)
        return;

    int old_value = slider->curr_value;
    int step = slider->step > 0 ? slider->step : 1;
    bool decrease = oxy_IsWidgetFocused(widget) &&
                    (slider->alignment == SCROLLBAR_VERTICAL ? event->scan_code == sk_Up : event->scan_code == sk_Left);
    bool increase = oxy_IsWidgetFocused(widget) &&
                    (slider->alignment == SCROLLBAR_VERTICAL ? event->scan_code == sk_Down : event->scan_code == sk_Right);

    if (event->primary_down && event->captured == widget)
    {
        int length = slider->alignment == SCROLLBAR_VERTICAL ? widget->size.height : widget->size.width;
        int position = slider->alignment == SCROLLBAR_VERTICAL
                           ? CURSOR_Y_POS - widget->position.y
                           : CURSOR_X_POS - widget->position.x;
        if (position < 0)
            position = 0;
        if (position > length)
            position = length;
        slider->curr_value = slider->min + (slider->max - slider->min) * position / length;
    }
    else
    {
        if (decrease)
            slider->curr_value -= step;
        if (increase)
            slider->curr_value += step;
    }

    if (slider->curr_value > slider->max)
        slider->curr_value = slider->max;
    if (slider->curr_value < slider->min)
        slider->curr_value = slider->min;
    if (old_value != slider->curr_value)
        widget->state.redraw = true;
}

void oxy_RenderSlider(struct oxy_widget_t *widget)
{
    struct oxy_slider_t *slider = (struct oxy_slider_t *)widget;

    if (!widget->state.visible)
        return;

    int range = slider->max - slider->min;
    int offset = slider->curr_value - slider->min;
    uint16_t track_width = slider->render_value && widget->size.width > 28 ? widget->size.width - 28 : widget->size.width;

    oxy_OutlinedRoundRectangle(widget->position.x, widget->position.y, track_width,
                               widget->size.height, widget->color.unselected,
                               widget->color.color_a);
    gfx_SetColor(widget->state.selected ? widget->color.selected : widget->color.color_b);
    if (range > 0 && track_width > 4)
        oxy_FillRoundRectangle(widget->position.x + 2, widget->position.y + 2,
                       (track_width - 4) * offset / range,
                       widget->size.height - 4, 0);

    if (slider->render_value && track_width < widget->size.width)
    {
        gfx_SetTextXY(widget->position.x + track_width + 3, widget->position.y + (widget->size.height - 8) / 2);
        gfx_PrintInt(slider->curr_value, 1);
    }

    widget->state.redraw = false;
}
