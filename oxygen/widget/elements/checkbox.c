#include "checkbox.h"

#include "../util.h"
#include "../loop.h"
#include "../color.h"
#include "../../oxy_cursor.h"
#include "../../oxy_gfx.h"

#include <graphx.h>
#include <keypadc.h>

void oxy_UpdateCheckbox(struct oxy_widget_t *widget)
{
	struct oxy_checkbox_t *checkbox = (struct oxy_checkbox_t *)widget;

	/*If the widget isn't visible then return right away*/
	if (!widget->state.visible)
		return;

	/* If the cursor is not overlapping with the widget check if it's clickable and prepare for a click */
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
			return;
		}
	}
	else
	{
		widget->state.selected = false;
		return;
	}

	if (kb_Data[6] & kb_Enter || kb_Data[1] & kb_2nd)
	{
		if (!widget->state.clicked && widget->state.selected)
		{
			widget->state.clicked = true;

			if (!checkbox->on)
			{
				checkbox->on = true;
			}
			else
			{
				checkbox->on = false;
			}

			// if (checkbox->click != NULL) checkbox->click(checkbox->arg);
		}
	}
	else
	{
		widget->state.clicked = false;
	}
}

void oxy_RenderCheckbox(struct oxy_widget_t *widget)
{
	struct oxy_checkbox_t *checkbox = (struct oxy_checkbox_t *)widget;

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

		if (checkbox->on)
		{
			// Change to a sprite later on
			gfx_SetColor(widget->color.color_a);
			gfx_FillRectangle(widget->position.x + 2, widget->position.y + 2, widget->size.width - 4, widget->size.height - 4);
		}
	}
}