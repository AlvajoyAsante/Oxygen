#include "switch.h"

#include "../util.h"
#include "../loop.h"
#include "../color.h"
#include "../../oxy_cursor.h"
#include "../../oxy_gfx.h"

#include <graphx.h>
#include <keypadc.h>

void oxy_UpdateSwitch(struct oxy_widget_t *widget)
{
	struct oxy_switch_t *switch_ = (struct oxy_switch_t *)widget;
	const struct oxy_input_event_t *event = oxy_GetInputEvent();

	if (!widget->state.visible)
		return;

	widget->state.selected = event->hovered == widget || oxy_IsWidgetFocused(widget);
	if (event->primary_pressed && event->hovered == widget)
	{
		oxy_FocusWidget(widget);
		oxy_CaptureWidget(widget);
	}
	if (event->primary_released && event->captured == widget && event->hovered == widget)
	{
		switch_->value = !switch_->value;
		widget->state.redraw = true;
	}
	widget->state.clicked = event->primary_down && event->captured == widget;
}

void oxy_RenderSwitch(struct oxy_widget_t *widget)
{
	struct oxy_switch_t *switch_ = (struct oxy_switch_t *)widget;

	if (widget->state.visible)
	{
		if (widget->state.selected)
		{
			if (widget->state.clicked)
			{
				oxy_OutlinedRectangle(widget->position.x, widget->position.y, widget->size.width, widget->size.height,
									  widget->color.clicked, widget->color.color_b);
			}
			else
			{
				oxy_OutlinedRectangle(widget->position.x, widget->position.y, widget->size.width, widget->size.height,
									  widget->color.selected, widget->color.color_b);
			}
		}
		else
		{
			oxy_OutlinedRectangle(widget->position.x, widget->position.y, widget->size.width, widget->size.height,
								  widget->color.unselected, widget->color.color_b);
		}

		if (!switch_->value)
		{
			gfx_SetColor(widget->color.unselected); // change to red
			gfx_FillRectangle(widget->position.x + 2, widget->position.y + 2, widget->size.width - 4, widget->size.height - 4);
		}
		else
		{
			gfx_SetColor(widget->color.unselected); // change to green
			gfx_FillRectangle(widget->position.x + 2, widget->position.y + 2, widget->size.width - 4, widget->size.height - 4);
		}
	}
}