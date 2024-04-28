#include "window.h"
#include "menu.h"

#include "../util.h"
#include "../loop.h"
#include "../color.h"
#include "../../oxy_cursor.h"
#include "../../oxy_gfx.h"

#include <graphx.h>
#include <string.h>
#include <math.h>

void oxy_UpdateWindow(struct oxy_widget_t *widget)
{
	if (widget->state.visible)
	{
		// if (kb_Data[6] & kb_Enter || kb_Data[1] & kb_2nd) {

		// }
	}
}

void oxy_RenderWindow(struct oxy_widget_t *widget)
{
	struct oxy_window_t *window = (struct oxy_window_t *)widget;

	if (window->widget.state.visible)
	{
		int x = window->widget.position.x;
		int y = window->widget.position.y;
		uint16_t width = window->widget.size.width;
		uint8_t height = window->widget.size.height;

		/* Title Area */
		if (widget->state.selected)
		{
			if (widget->state.clicked)
			{
				gfx_SetColor(widget->color.clicked);
			}
			else
			{
				gfx_SetColor(widget->color.selected);
			}
		}
		else
		{
			gfx_SetColor(widget->color.unselected);
		}
		oxy_FillRoundRectangle(x, y, width, height + 12, 0);

		/* Title Text */
		if (widget->state.selected)
		{
			if (widget->state.clicked)
			{
				oxy_SetTextColor(widget->color.text_fg_clicked, widget->color.text_bg_clicked);
			}
			else
			{
				oxy_SetTextColor(widget->color.text_fg_selected, widget->color.text_bg_selected);
			}
		}
		else
		{
			oxy_SetTextColor(widget->color.text_fg_unselected, widget->color.text_bg_unselected);
		}
		gfx_PrintStringXY(window->title, 160 - gfx_GetStringWidth(window->title) / 2, y + (12 / 2) - 3);

		/* Widget Area */
		gfx_SetColor(widget->color.color_a);
		oxy_FillRoundRectangle(x + 1, y + 12 + 1, width - 2, height - 2, 0);
	}
}

void oxy_ResizeWindow(struct oxy_widget_t *widget, uint16_t width, uint8_t height)
{
	int i = 0;
	if (widget->resize.resizable)
	{
		if (width < widget->resize.min_width)
		{
			width = widget->resize.min_width;
		}
		else
			widget->size.width = width;

		if (height < widget->resize.min_height)
		{
			height = widget->resize.min_height;
		}
		else
			widget->size.height = height;

		while (widget->child[i])
		{
			struct oxy_widget_t *child = widget->child[i];

			if (child->type == OXY_MENU_TYPE)
			{
				struct oxy_menu_t *menu = (struct oxy_menu_t *)child;
				if (!menu->widget.position.x_align)
				{
					menu->columns = width / menu->widget.resize.min_width;
					menu->widget.size.width = width;
				}
				if (!menu->widget.position.y_align)
				{
					menu->rows = height / menu->widget.resize.min_height;
					menu->widget.size.height = height;
				}
			}
			else
			{
				child->size.width = width;
				child->size.height = height;
			}
			i++;
		}

		oxy_AlignChildren(widget);
	}
}

bool oxy_IsWindowVisible(struct oxy_window_t *window)
{
	// Check if the window is within the boundaries of the screen
	if (window->widget.position.x < 0 || window->widget.position.y < 0 || window->widget.position.x + window->widget.size.width > LCD_WIDTH || window->widget.position.y + window->widget.size.height > LCD_WIDTH)
	{
		return false;
	}

	// Check if the window is obscured by another window
	for (struct oxy_widget_t **w = oxy_config->stack; *w; ++w)
	{
		struct oxy_widget_t *p = *w;

		// make sure that widget is not the same as the window and make widget is a window
		if (p != &window->widget && p->type == OXY_WINDOW_TYPE && p->state.visible && !oxy_IsWindowViewable((struct oxy_window_t *)p, window))
		{
			return false;
		}
	}

	return true;
}

bool oxy_IsWindowViewable(struct oxy_window_t *window, struct oxy_window_t *reference)
{
	return oxy_CheckWidgetOverlap(&(window->widget), &(reference->widget));
}