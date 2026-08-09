#include "label.h"

#include "../util.h"
#include "../loop.h"
#include "../color.h"
#include "../../oxy_cursor.h"
#include "../../oxy_gfx.h"
#include "../../oxy_gtk.h"

void oxy_UpdateLabel(struct oxy_widget_t *widget)
{
	if (widget->state.visible)
	{
		if (CURSOR_STATE_ID == CURSOR_STATE_IDLE && oxy_CheckCursorOverlap(widget))
		{
			if (widget->state.clickable)
			{
				widget->state.selected = true;
				CURSOR_STATE_ID = CURSOR_STATE_CLICK;
			}
			else
			{
				// Not clickable so move on
				widget->state.selected = false;
				return;
			}
		}
		else
		{
			// Not clickable so move on
			widget->state.selected = false;
			return;
		}
	}
}

void oxy_RenderLabel(struct oxy_widget_t *widget)
{
	struct oxy_label_t *label = (struct oxy_label_t *)widget;
	uint8_t text_fg;
	uint8_t text_bg;

	if (widget->state.visible && label->text)
	{
		if (widget->state.clicked)
		{
			text_fg = widget->color.text_fg_clicked;
			text_bg = widget->color.text_bg_clicked;
		}
		else if (widget->state.selected)
		{
			text_fg = widget->color.text_fg_selected;
			text_bg = widget->color.text_bg_selected;
		}
		else
		{
			text_fg = widget->color.text_fg_unselected;
			text_bg = widget->color.text_bg_unselected;
		}
		oxy_SetTextColor(text_fg, text_bg);
		if (label->wrap)
		{
			oxy_PrintWordWrap(label->text,
							  widget->position.x,
							  widget->position.y,
							  widget->size.width,
							  label->max_lines <= 0 ? 1 : label->max_lines,
							  label->start_line < 0 ? 0 : label->start_line);
		}
		else
		{
			gfx_PrintStringXY(label->text, widget->position.x, widget->position.y);
		}
	}
}

void oxy_SetLabelText(struct oxy_widget_t *widget, const char *text)
{
	struct oxy_label_t *label = (struct oxy_label_t *)widget;
	label->text = (char *)text;
	widget->size.width = text ? gfx_GetStringWidth(text) : 0;
	widget->state.redraw = true;
}