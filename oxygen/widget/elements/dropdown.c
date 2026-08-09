#include "dropdown.h"

#include "../util.h"
#include "../loop.h"
#include "../color.h"
#include "../../oxy_cursor.h"
#include "../../oxy_gfx.h"

#include <graphx.h>
#include <keypadc.h>

void oxy_RenderDropDown(struct oxy_widget_t *widget)
{
    struct oxy_dropdown_t *dropdown = (struct oxy_dropdown_t *)widget;

    if (!widget->state.visible)
        return;

    oxy_OutlinedRectangle(widget->position.x, widget->position.y,
                          widget->size.width, widget->size.height,
                          widget->state.selected ? widget->color.selected : widget->color.unselected,
                          widget->color.color_a);

    if (dropdown->button && dropdown->button->render)
    {
        dropdown->button->position = widget->position;
        dropdown->button->size = widget->size;
        dropdown->button->state.visible = true;
        dropdown->button->state.selected = widget->state.selected;
        dropdown->button->render(dropdown->button);
    }
    else if (dropdown->menu && dropdown->menu->text && dropdown->menu->text[dropdown->menu->selection])
    {
        char *text = dropdown->menu->text[dropdown->menu->selection];
        oxy_SetTextColor(widget->color.text_fg_unselected, widget->color.text_bg_unselected);
        gfx_PrintStringXY(text,
                          widget->position.x + (widget->size.width - gfx_GetStringWidth(text)) / 2,
                          widget->position.y + (widget->size.height - 8) / 2);
    }

    if (dropdown->menu && dropdown->menu->widget.state.visible)
        oxy_RenderMenu(&dropdown->menu->widget);

    widget->state.redraw = false;
}

void oxy_UpdateDropDown(struct oxy_widget_t *widget)
{
    struct oxy_dropdown_t *dropdown = (struct oxy_dropdown_t *)widget;
    const struct oxy_input_event_t *event = oxy_GetInputEvent();

    if (!widget->state.visible)
    {
        if (dropdown->menu)
            dropdown->menu->widget.state.visible = false;
        return;
    }

    bool over_button = oxy_CheckCursorOverlap(widget);
    bool over_menu = dropdown->menu && dropdown->menu->widget.state.visible &&
                     oxy_CheckCursorOverlap(&dropdown->menu->widget);

    widget->state.selected = over_button || oxy_IsWidgetFocused(widget);
    if (event->primary_pressed && over_button)
    {
        oxy_FocusWidget(widget);
        oxy_CaptureWidget(widget);
    }

    if (dropdown->menu && event->primary_released && event->captured == widget)
    {
        if (over_menu)
        {
            dropdown->menu->widget.state.visible = false;
            widget->state.redraw = true;
        }
        else if (over_button)
        {
            dropdown->menu->widget.state.visible = !dropdown->menu->widget.state.visible;
            widget->state.redraw = true;
        }
    }

    if (dropdown->menu && dropdown->menu->widget.state.visible && oxy_IsWidgetFocused(widget))
    {
        int selection = dropdown->menu->selection;
        int count = dropdown->menu->rows * dropdown->menu->columns;
        if (event->scan_code == sk_Up && selection >= dropdown->menu->columns)
            selection -= dropdown->menu->columns;
        if (event->scan_code == sk_Down && selection + dropdown->menu->columns < count)
            selection += dropdown->menu->columns;
        if (event->scan_code == sk_Left && selection > 0) selection--;
        if (event->scan_code == sk_Right && selection + 1 < count) selection++;
        if (selection != dropdown->menu->selection)
        {
            dropdown->menu->selection = selection;
            widget->state.redraw = true;
        }
        if (event->scan_code == sk_Clear)
        {
            dropdown->menu->widget.state.visible = false;
            widget->state.redraw = true;
        }
    }

    widget->state.clicked = event->primary_down && event->captured == widget;
    if (dropdown->menu && dropdown->menu->widget.state.visible)
        oxy_UpdateMenu(&dropdown->menu->widget);
}
