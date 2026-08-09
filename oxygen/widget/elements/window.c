#include "window.h"
#include "menu.h"

#include "../util.h"
#include "../loop.h"
#include "../color.h"
#include "../../oxy_cursor.h"
#include "../../oxy_gfx.h"

#include <graphx.h>
#include <keypadc.h>
#include <string.h>
#include <math.h>

enum {
	OXY_WINDOW_DRAG_NONE,
	OXY_WINDOW_DRAG_MOVE,
	OXY_WINDOW_DRAG_RESIZE
};

static struct {
	struct oxy_window_t *focused;
	struct oxy_window_t *captured;
	oxy_window_background_callback_t render_background;
	void *background_arg;
	int drag_offset_x;
	int drag_offset_y;
	uint8_t drag_mode;
	bool pointer_down;
	bool dirty;
} oxy_window_manager;

static bool oxy_WindowContains(const struct oxy_window_t *window, int x, int y)
{
	int height;
	if (!window || !window->widget.state.visible) return false;
	height = window->minimized ? 12 : window->widget.size.height + 12;
	return x >= window->widget.position.x &&
		x < window->widget.position.x + window->widget.size.width &&
		y >= window->widget.position.y &&
		y < window->widget.position.y + height;
}

static void oxy_InvalidateWindowManager(void)
{
	oxy_window_manager.dirty = true;
	if (oxy_config) oxy_config->render.needs_redraw = true;
}

static int oxy_WindowControlAt(struct oxy_window_t *window, int x, int y)
{
	if (y < window->widget.position.y || y >= window->widget.position.y + 12)
		return 0;
	int right = window->widget.position.x + window->widget.size.width;
	if (window->closeable && x >= right - 14 && x < right) return 3;
	if (window->maximizable && x >= right - 28 && x < right - 14) return 2;
	if (window->minimizable && x >= right - 42 && x < right - 28) return 1;
	return 0;
}

void oxy_UpdateWindow(struct oxy_widget_t *widget)
{
	struct oxy_window_t *window = (struct oxy_window_t *)widget;
	if (!widget->state.visible)
		return;

	widget->state.selected = oxy_CheckCursorOverlap(widget);
	if (!window->minimized && widget->child &&
		(oxy_window_manager.focused == window ||
		 oxy_GetWindowAt(CURSOR_X_POS, CURSOR_Y_POS) == window))
		oxy_UpdateStack(widget->child);
}

void oxy_SetWindowManagerBackground(oxy_window_background_callback_t callback,
									void *arg)
{
	oxy_window_manager.render_background = callback;
	oxy_window_manager.background_arg = arg;
	oxy_window_manager.dirty = true;
}

struct oxy_window_t *oxy_GetWindowAt(int x, int y)
{
	int index;
	int length;
	if (!oxy_config || !oxy_config->stack) return NULL;
	length = oxy_GetStackLength(oxy_config->stack);
	for (index = length - 1; index >= 0; index--) {
		struct oxy_widget_t *widget = oxy_config->stack[index];
		if (widget->type == OXY_WINDOW_TYPE &&
			oxy_WindowContains((struct oxy_window_t *)widget, x, y))
			return (struct oxy_window_t *)widget;
	}
	return NULL;
}

struct oxy_window_t *oxy_GetFocusedWindow(void)
{
	return oxy_window_manager.focused;
}

bool oxy_BringWindowToFront(struct oxy_window_t *window)
{
	int index;
	int length;
	if (!window || !oxy_config || !oxy_config->stack) return false;
	length = oxy_GetStackLength(oxy_config->stack);
	index = oxy_GetWidgetStackIndex(oxy_config->stack, &window->widget);
	if (index < 0) return false;
	for (; index + 1 < length; index++)
		oxy_config->stack[index] = oxy_config->stack[index + 1];
	oxy_config->stack[length - 1] = &window->widget;
	oxy_window_manager.focused = window;
	oxy_InvalidateWindowManager();
	return true;
}

bool oxy_MoveWindow(struct oxy_window_t *window, int x, int y)
{
	if (!window || window->maximized) return false;
	if (window->widget.size.width >= LCD_WIDTH) x = 0;
	if (x < 0) x = 0;
	if (y < 0) y = 0;
	if (window->widget.size.width < LCD_WIDTH &&
		x + window->widget.size.width > LCD_WIDTH)
		x = LCD_WIDTH - window->widget.size.width;
	if (!window->minimized && window->widget.size.height + 12 >= LCD_HEIGHT)
		y = 0;
	else if (y + 12 > LCD_HEIGHT)
		y = LCD_HEIGHT - 12;
	else if (!window->minimized && y + window->widget.size.height + 12 > LCD_HEIGHT)
		y = LCD_HEIGHT - window->widget.size.height - 12;
	if (window->widget.position.x == x && window->widget.position.y == y) return false;
	window->widget.position.x = x;
	window->widget.position.y = y;
	if (!window->minimized) window->restore_position = window->widget.position;
	oxy_InvalidateWindowManager();
	return true;
}

bool oxy_UpdateWindowManager(bool pointer_down)
{
	bool pressed = pointer_down && !oxy_window_manager.pointer_down;
	bool handled = oxy_window_manager.captured != NULL;

	if (pressed) {
		struct oxy_window_t *window = oxy_GetWindowAt(CURSOR_X_POS, CURSOR_Y_POS);
		oxy_window_manager.captured = NULL;
		oxy_window_manager.drag_mode = OXY_WINDOW_DRAG_NONE;
		if (window) {
			int control;
			oxy_BringWindowToFront(window);
			control = oxy_WindowControlAt(window, CURSOR_X_POS, CURSOR_Y_POS);
			if (control == 1) {
				oxy_MinimizeWindow(window);
				handled = true;
			} else if (control == 2) {
				oxy_MaximizeWindow(window);
				handled = true;
			} else if (control == 3) {
				window->widget.state.visible = false;
				if (window->on_close) window->on_close(NULL);
				handled = true;
			} else if (!window->minimized && !window->maximized &&
					   window->widget.resize.resizable &&
					   CURSOR_X_POS >= window->widget.position.x + window->widget.size.width - 12 &&
					   CURSOR_Y_POS >= window->widget.position.y + window->widget.size.height) {
				oxy_window_manager.captured = window;
				oxy_window_manager.drag_mode = OXY_WINDOW_DRAG_RESIZE;
				handled = true;
			} else if (!window->maximized &&
					   CURSOR_Y_POS < window->widget.position.y + 12) {
				oxy_window_manager.captured = window;
				oxy_window_manager.drag_mode = OXY_WINDOW_DRAG_MOVE;
				oxy_window_manager.drag_offset_x = CURSOR_X_POS - window->widget.position.x;
				oxy_window_manager.drag_offset_y = CURSOR_Y_POS - window->widget.position.y;
				handled = true;
			}
		} else {
			oxy_window_manager.focused = NULL;
		}
	}

	if (pointer_down && oxy_window_manager.captured) {
		if (oxy_window_manager.drag_mode == OXY_WINDOW_DRAG_MOVE) {
			oxy_MoveWindow(oxy_window_manager.captured,
				CURSOR_X_POS - oxy_window_manager.drag_offset_x,
				CURSOR_Y_POS - oxy_window_manager.drag_offset_y);
		} else if (oxy_window_manager.drag_mode == OXY_WINDOW_DRAG_RESIZE) {
			oxy_ResizeWindow(&oxy_window_manager.captured->widget,
				CURSOR_X_POS - oxy_window_manager.captured->widget.position.x,
				CURSOR_Y_POS - oxy_window_manager.captured->widget.position.y - 12);
			oxy_InvalidateWindowManager();
		}
	}

	if (!pointer_down) {
		oxy_window_manager.captured = NULL;
		oxy_window_manager.drag_mode = OXY_WINDOW_DRAG_NONE;
	}
	oxy_window_manager.pointer_down = pointer_down;
	return handled;
}

void oxy_RenderWindowBackground(void)
{
	if (oxy_window_manager.render_background)
		oxy_window_manager.render_background(oxy_window_manager.background_arg);
	oxy_window_manager.dirty = false;
}

bool oxy_WindowManagerNeedsRedraw(void)
{
	return oxy_window_manager.dirty;
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
		oxy_FillRoundRectangle(x, y, width, window->minimized ? 12 : height + 12, 0);

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
		if (window->title)
			gfx_PrintStringXY(window->title, x + (width - gfx_GetStringWidth(window->title)) / 2, y + 3);
		if (window->minimizable) gfx_PrintStringXY("_", x + width - 39, y + 2);
		if (window->maximizable) gfx_PrintStringXY(window->maximized ? "o" : "+", x + width - 25, y + 2);
		if (window->closeable) gfx_PrintStringXY("X", x + width - 11, y + 2);

		/* Widget Area */
		if (!window->minimized)
		{
			gfx_SetColor(widget->color.color_a);
			oxy_FillRoundRectangle(x + 1, y + 13, width - 2, height - 2, 0);

			if (widget->child)
				oxy_RenderStack(widget->child);
		}
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
		if (widget->resize.max_width && width > widget->resize.max_width)
			width = widget->resize.max_width;
		widget->size.width = width;

		if (height < widget->resize.min_height)
		{
			height = widget->resize.min_height;
		}
		if (widget->resize.max_height && height > widget->resize.max_height)
			height = widget->resize.max_height;
		widget->size.height = height;

		while (widget->child && widget->child[i])
		{
			struct oxy_widget_t *child = widget->child[i];

			if (child->type == OXY_MENU_TYPE)
			{
				struct oxy_menu_t *menu = (struct oxy_menu_t *)child;
				if (!menu->widget.position.x_align && menu->widget.resize.min_width)
				{
					menu->columns = width / menu->widget.resize.min_width;
					menu->widget.size.width = width;
				}
				if (!menu->widget.position.y_align && menu->widget.resize.min_height)
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
		oxy_InvalidateWindowManager();
	}
}

void oxy_SetWindowFeatures(struct oxy_window_t *window, bool closeable,
							 bool minimizable, bool maximizable, bool resizable)
{
	if (!window) return;
	window->closeable = closeable;
	window->minimizable = minimizable;
	window->maximizable = maximizable;
	window->widget.resize.resizable = resizable;
	oxy_InvalidateWindowManager();
}

bool oxy_RestoreWindow(struct oxy_window_t *window)
{
	if (!window || (!window->minimized && !window->maximized)) return false;
	window->widget.position = window->restore_position;
	window->widget.size = window->restore_size;
	window->minimized = false;
	window->maximized = false;
	oxy_InvalidateWindowManager();
	return true;
}

bool oxy_MinimizeWindow(struct oxy_window_t *window)
{
	if (!window || !window->minimizable) return false;
	if (window->minimized) return oxy_RestoreWindow(window);
	if (!window->maximized) {
		window->restore_position = window->widget.position;
		window->restore_size = window->widget.size;
	}
	window->minimized = true;
	window->maximized = false;
	oxy_InvalidateWindowManager();
	return true;
}

bool oxy_MaximizeWindow(struct oxy_window_t *window)
{
	if (!window || !window->maximizable) return false;
	if (window->maximized) return oxy_RestoreWindow(window);
	if (!window->minimized) {
		window->restore_position = window->widget.position;
		window->restore_size = window->widget.size;
	}
	window->widget.position.x = 0;
	window->widget.position.y = 0;
	window->widget.size.width = LCD_WIDTH;
	window->widget.size.height = LCD_HEIGHT - 12;
	window->minimized = false;
	window->maximized = true;
	oxy_InvalidateWindowManager();
	return true;
}

bool oxy_IsWindowVisible(struct oxy_window_t *window)
{
	if (!window || !oxy_config)
		return false;

	// Check if the window is within the boundaries of the screen
	if (!window->widget.state.visible || window->widget.position.x < 0 || window->widget.position.y < 0 || window->widget.position.x + window->widget.size.width > LCD_WIDTH || window->widget.position.y + window->widget.size.height + 12 > LCD_HEIGHT)
	{
		return false;
	}

	// Check if the window is obscured by another window
	for (struct oxy_widget_t **w = oxy_config->stack; w && *w; ++w)
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