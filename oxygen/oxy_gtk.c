#include "oxy_gfx.h"
#include "oxy_gtk.h"
#include "oxy_mouse.h"
#include "oxy_widget.h"

#include <tice.h>
#include <debug.h>
#include <graphx.h>
#include <keypadc.h>
#include <string.h>
#include <ctype.h>
#include <fileioc.h>
#include <stdlib.h>

struct gtk_wm_t gtk_wm;

enum {
	OXY_GTK_DRAG_NONE,
	OXY_GTK_DRAG_MOVE,
	OXY_GTK_DRAG_RESIZE,
	OXY_GTK_DRAG_SCROLL
};

static int oxy_GtkViewportHeight(const struct gtk_window_t *window)
{
	int height = window->widget.size.height - 20;
	return height > 1 ? height : 1;
}

static int oxy_GtkMaxScroll(const struct gtk_window_t *window)
{
	int maximum = window->content_height - oxy_GtkViewportHeight(window);
	return maximum > 0 ? maximum : 0;
}

static bool oxy_GtkScrollbarContains(const struct gtk_window_t *window,
									 int x, int y)
{
	return !window->minimized && oxy_GtkMaxScroll(window) > 0 &&
		x >= window->widget.position.x + window->widget.size.width - 16 &&
		x < window->widget.position.x + window->widget.size.width - 2 &&
		y >= window->widget.position.y + 16 &&
		y < window->widget.position.y + 16 + oxy_GtkViewportHeight(window);
}

static bool oxy_GtkWindowContains(const struct gtk_window_t *window, int x, int y)
{
	int height;
	if (!window || !window->widget.state.visible) return false;
	height = window->minimized ? 12 : window->widget.size.height + 12;
	return x >= window->widget.position.x &&
		x < window->widget.position.x + window->widget.size.width &&
		y >= window->widget.position.y &&
		y < window->widget.position.y + height;
}

static uint8_t oxy_VisibleGtkWindowCount(void)
{
	uint8_t count = 0;
	uint8_t index;
	for (index = 0; index < gtk_wm.amount; index++) {
		if (gtk_wm.windows[index]->widget.state.visible) count++;
	}
	return count;
}

static bool oxy_RegisterGtkWindow(struct gtk_window_t *window)
{
	if (!window || gtk_wm.amount >= OXY_GTK_MAX_WINDOWS) return false;
	gtk_wm.windows[gtk_wm.amount++] = window;
	gtk_wm.focused = window;
	gtk_wm.dirty = true;
	return true;
}

static void oxy_UnregisterGtkWindow(struct gtk_window_t *window)
{
	uint8_t index;
	for (index = 0; index < gtk_wm.amount; index++) {
		if (gtk_wm.windows[index] == window) {
			for (; index + 1 < gtk_wm.amount; index++)
				gtk_wm.windows[index] = gtk_wm.windows[index + 1];
			gtk_wm.windows[--gtk_wm.amount] = NULL;
			break;
		}
	}
	if (gtk_wm.focused == window)
		gtk_wm.focused = gtk_wm.amount ? gtk_wm.windows[gtk_wm.amount - 1] : NULL;
	if (gtk_wm.captured == window) {
		gtk_wm.captured = NULL;
		gtk_wm.drag_mode = OXY_GTK_DRAG_NONE;
	}
	gtk_wm.dirty = true;
}

// Windows
struct gtk_window_t* oxy_CreateWindow(char title[9], int x, uint8_t y, int width, int height)
{
	struct gtk_window_t *window = malloc(sizeof(struct gtk_window_t));
	struct gtk_widget_t widget;

	if (window == NULL) return NULL;
	
	strncpy(window->title, title, 9);
	window->title[8] = '\0';
	strncpy(window->wm_class, "PARENT", 9);
	
	widget.type = GTK_WINDOW_TYPE;
	widget.position.x = x;
	widget.position.y = y;
	widget.size.width = width;
	widget.size.height = height;
	widget.state.visible = true;
	widget.child = NULL;
	
	window->widget = widget;
	window->restore_position = widget.position;
	window->restore_size = widget.size;
	window->min_width = 80;
	window->min_height = 40;
	window->closeable = true;
	window->minimizable = true;
	window->maximizable = true;
	window->resizable = true;
	window->minimized = false;
	window->maximized = false;
	window->backing_left = NULL;
	window->backing_right = NULL;
	window->backing_valid = false;
	window->render_content = NULL;
	window->render_arg = NULL;
	window->hit_test_content = NULL;
	window->hit_test_arg = NULL;
	window->content_height = 0;
	window->scroll_offset = 0;
	if (!oxy_RegisterGtkWindow(window)) {
		free(window);
		return NULL;
	}
	
	return window;
}

void oxy_DestroyWindow(struct gtk_window_t *window)
{
	if (!window) return;
	oxy_ReleaseGtkWindowBackground(window);
	oxy_UnregisterGtkWindow(window);
	free(window);
}

void oxy_UpdateGtkWindow(struct gtk_window_t *window)
{	
	(void)window;
}

void oxy_SetGtkWindowRenderer(struct gtk_window_t *window,
							  oxy_gtk_window_callback_t callback, void *arg)
{
	if (!window) return;
	window->render_content = callback;
	window->render_arg = arg;
	gtk_wm.dirty = true;
}

void oxy_SetGtkWindowHitTest(struct gtk_window_t *window,
							 oxy_gtk_hit_test_callback_t callback, void *arg)
{
	if (!window) return;
	window->hit_test_content = callback;
	window->hit_test_arg = arg;
}

void oxy_SetGtkWindowContentHeight(struct gtk_window_t *window, int height)
{
	if (!window) return;
	window->content_height = height > 0 ? height : 0;
	if (window->scroll_offset > oxy_GtkMaxScroll(window))
		window->scroll_offset = oxy_GtkMaxScroll(window);
	gtk_wm.dirty = true;
}

int oxy_GetGtkWindowScrollOffset(const struct gtk_window_t *window)
{
	return window ? window->scroll_offset : 0;
}

bool oxy_ScrollGtkWindowTo(struct gtk_window_t *window, int offset)
{
	int maximum;
	if (!window) return false;
	maximum = oxy_GtkMaxScroll(window);
	if (offset < 0) offset = 0;
	if (offset > maximum) offset = maximum;
	if (offset == window->scroll_offset) return false;
	window->scroll_offset = offset;
	gtk_wm.dirty = true;
	return true;
}

static int oxy_GtkFormRowHeight(const struct oxy_gtk_form_t *form)
{
	return form->window->widget.size.width < 210 ? 38 : 29;
}

static int oxy_GtkFormContentHeight(const struct oxy_gtk_form_t *form)
{
	return oxy_GtkFormRowHeight(form) * form->row_count +
		(form->action_button ? 34 : 0);
}

void oxy_InitGtkForm(struct oxy_gtk_form_t *form, struct gtk_window_t *window,
					 struct oxy_gtk_form_row_t *rows, uint8_t row_count,
					 struct oxy_widget_t *action_button)
{
	if (!form) return;
	form->window = window;
	form->rows = rows;
	form->row_count = row_count;
	form->selected_row = 0;
	form->selection_color = window ? window->colors.text_fg : 0;
	form->action_button = action_button;
	if (window) oxy_SetGtkWindowContentHeight(window,
		oxy_GtkFormContentHeight(form));
}

int oxy_GtkFormRowAt(const struct oxy_gtk_form_t *form, int x, int y)
{
	int viewport_top;
	int viewport_height;
	int content_x;
	int content_width;
	int row_height;
	int scroll_offset;
	int row;
	if (!form || !form->window || form->window->minimized) return -1;
	viewport_top = form->window->widget.position.y + 16;
	viewport_height = oxy_GtkViewportHeight(form->window);
	content_x = form->window->widget.position.x + 8;
	content_width = form->window->widget.size.width - 24;
	row_height = oxy_GtkFormRowHeight(form);
	scroll_offset = form->window->scroll_offset;
	if (x < content_x || x >= content_x + content_width ||
		y < viewport_top || y >= viewport_top + viewport_height) return -1;
	row = (y - viewport_top + scroll_offset) / row_height;
	if (row >= 0 && row < form->row_count) return row;
	if (form->action_button &&
		y >= viewport_top + row_height * form->row_count - scroll_offset + 3 &&
		y < viewport_top + row_height * form->row_count - scroll_offset + 29)
		return form->row_count;
	return -1;
}

bool oxy_GtkFormHitTest(struct gtk_window_t *window, void *arg, int x, int y)
{
	struct oxy_gtk_form_t *form = arg;
	(void)window;
	return oxy_GtkFormRowAt(form, x, y) >= 0;
}

void oxy_SetGtkFormSelection(struct oxy_gtk_form_t *form, uint8_t row)
{
	if (!form || row > form->row_count) return;
	form->selected_row = row;
	if (form->window) gtk_wm.dirty = true;
}

void oxy_SetGtkFormSelectionColor(struct oxy_gtk_form_t *form, uint8_t color)
{
	if (!form) return;
	form->selection_color = color;
	if (form->window) gtk_wm.dirty = true;
}

bool oxy_EnsureGtkFormSelectionVisible(struct oxy_gtk_form_t *form)
{
	int row_height;
	int top;
	int bottom;
	int viewport_height;
	int scroll_offset;
	if (!form || !form->window) return false;
	row_height = oxy_GtkFormRowHeight(form);
	top = form->selected_row * row_height;
	bottom = top + (form->selected_row == form->row_count ? 28 : row_height);
	viewport_height = oxy_GtkViewportHeight(form->window);
	scroll_offset = form->window->scroll_offset;
	if (top < scroll_offset)
		return oxy_ScrollGtkWindowTo(form->window, top);
	if (bottom > scroll_offset + viewport_height)
		return oxy_ScrollGtkWindowTo(form->window,
			bottom - viewport_height);
	return false;
}

void oxy_RenderGtkForm(struct gtk_window_t *window, void *arg)
{
	struct oxy_gtk_form_t *form = arg;
	int viewport_top;
	int viewport_height;
	int content_x;
	int content_width;
	int row_height;
	int scroll_offset;
	uint8_t row;
	if (!form || !window) return;
	form->window = window;
	oxy_SetGtkWindowContentHeight(window, oxy_GtkFormContentHeight(form));
	viewport_top = window->widget.position.y + 16;
	viewport_height = oxy_GtkViewportHeight(window);
	content_x = window->widget.position.x + 8;
	content_width = window->widget.size.width - 24;
	row_height = oxy_GtkFormRowHeight(form);
	scroll_offset = window->scroll_offset;
	gfx_SetClipRegion(window->widget.position.x + 2, viewport_top,
		window->widget.position.x + window->widget.size.width - 2,
		viewport_top + viewport_height);
	gfx_SetTextConfig(gfx_text_clip);
	for (row = 0; row < form->row_count; row++) {
		struct oxy_gtk_form_row_t *form_row = &form->rows[row];
		int top = viewport_top + row * row_height - scroll_offset;
		if (row == form->selected_row) {
			gfx_SetColor(form->selection_color);
			oxy_RoundRectangle(content_x, top, content_width, row_height - 2, 0);
		}
		if (form_row->control) {
			form_row->control->state.selected = row == form->selected_row;
			if (form_row->control->type == OXY_CHECKBOX_TYPE) {
				oxy_SetWidgetPosition(form_row->control, content_x + 3, top + 7);
				oxy_SetWidgetSize(form_row->control, 12, 12);
			} else if (form_row->control->type == OXY_SLIDER_TYPE) {
				oxy_SetWidgetPosition(form_row->control, content_x + 3, top + 16);
				oxy_SetWidgetSize(form_row->control, content_width - 6, 11);
			}
			form_row->control->render(form_row->control);
		}
		if (form_row->label) {
			int label_x = content_x +
				(form_row->control && form_row->control->type == OXY_CHECKBOX_TYPE
				 ? 22 : 3);
			oxy_SetWidgetPosition(&form_row->label->widget, label_x, top + 5);
			oxy_SetWidgetSize(&form_row->label->widget,
				content_x + content_width - label_x - 3,
				row_height - 7);
			form_row->label->wrap = true;
			form_row->label->max_lines = row_height / 9;
			form_row->label->widget.state.selected =
				row == form->selected_row;
			form_row->label->widget.state.clicked = false;
			form_row->label->widget.render(&form_row->label->widget);
		}
	}
	if (form->action_button) {
		int top = viewport_top + row_height * form->row_count -
			scroll_offset + 3;
		form->action_button->state.selected =
			form->selected_row == form->row_count;
		oxy_SetWidgetPosition(form->action_button, content_x, top);
		oxy_SetWidgetSize(form->action_button, content_width, 26);
		if (form->action_button->type == OXY_BUTTON_TYPE) {
			struct oxy_button_t *button = (struct oxy_button_t *)form->action_button;
			if (button->label) {
				oxy_SetWidgetPosition(&button->label->widget,
					content_x + (content_width -
					gfx_GetStringWidth(button->label->text)) / 2, top + 9);
			}
		}
		form->action_button->render(form->action_button);
	}
	gfx_SetClipRegion(0, 0, LCD_WIDTH, LCD_HEIGHT);
	gfx_SetTextConfig(gfx_text_noclip);
}

void oxy_SetGtkWindowManagerBackground(oxy_gtk_background_callback_t callback,
									 void *arg)
{
	gtk_wm.render_background = callback;
	gtk_wm.background_arg = arg;
	gtk_wm.dirty = true;
}

struct gtk_window_t *oxy_GetFocusedGtkWindow(void)
{
	return gtk_wm.focused;
}

struct gtk_window_t *oxy_GetGtkWindowAt(int x, int y)
{
	int index;
	for (index = gtk_wm.amount - 1; index >= 0; index--) {
		if (oxy_GtkWindowContains(gtk_wm.windows[index], x, y))
			return gtk_wm.windows[index];
	}
	return NULL;
}

bool oxy_BringGtkWindowToFront(struct gtk_window_t *window)
{
	uint8_t index;
	if (!window) return false;
	for (index = 0; index < gtk_wm.amount; index++) {
		if (gtk_wm.windows[index] == window) {
			for (; index + 1 < gtk_wm.amount; index++)
				gtk_wm.windows[index] = gtk_wm.windows[index + 1];
			gtk_wm.windows[gtk_wm.amount - 1] = window;
			gtk_wm.focused = window;
			gtk_wm.dirty = true;
			return true;
		}
	}
	return false;
}

bool oxy_MoveGtkWindow(struct gtk_window_t *window, int x, int y)
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
	gtk_wm.dirty = true;
	return true;
}

bool oxy_UpdateGtkWindowManager(int pointer_x, int pointer_y, bool pointer_down)
{
	bool pressed = pointer_down && !gtk_wm.pointer_down;
	bool handled = gtk_wm.captured != NULL;
	struct gtk_window_t *hovered = oxy_GetGtkWindowAt(pointer_x, pointer_y);

	if (pressed) {
		struct gtk_window_t *window = hovered;
		gtk_wm.captured = NULL;
		gtk_wm.drag_mode = OXY_GTK_DRAG_NONE;
		if (window) {
			enum oxy_window_control_t control;
			oxy_BringGtkWindowToFront(window);
			control = oxy_GetGtkWindowControlAt(window, pointer_x, pointer_y);
			if (control != OXY_WINDOW_CONTROL_NONE) {
				oxy_ActivateGtkWindowControl(window, control);
				handled = true;
			} else if (oxy_GtkScrollbarContains(window, pointer_x, pointer_y)) {
				gtk_wm.captured = window;
				gtk_wm.drag_mode = OXY_GTK_DRAG_SCROLL;
				handled = true;
			} else if (!window->minimized && !window->maximized && window->resizable &&
					   pointer_x >= window->widget.position.x + window->widget.size.width - 12 &&
					   pointer_y >= window->widget.position.y + window->widget.size.height) {
				gtk_wm.captured = window;
				gtk_wm.drag_mode = OXY_GTK_DRAG_RESIZE;
				handled = true;
			} else if (!window->maximized &&
					   pointer_y < window->widget.position.y + 12) {
				gtk_wm.captured = window;
				gtk_wm.drag_mode = OXY_GTK_DRAG_MOVE;
				gtk_wm.drag_offset_x = pointer_x - window->widget.position.x;
				gtk_wm.drag_offset_y = pointer_y - window->widget.position.y;
				handled = true;
			}
		} else {
			gtk_wm.focused = NULL;
		}
	}

	if (pointer_down && gtk_wm.captured) {
		if (gtk_wm.drag_mode == OXY_GTK_DRAG_MOVE) {
			oxy_MoveGtkWindow(gtk_wm.captured,
				pointer_x - gtk_wm.drag_offset_x,
				pointer_y - gtk_wm.drag_offset_y);
		} else if (gtk_wm.drag_mode == OXY_GTK_DRAG_RESIZE) {
			oxy_ResizeGtkWindow(gtk_wm.captured,
				pointer_x - gtk_wm.captured->widget.position.x,
				pointer_y - gtk_wm.captured->widget.position.y - 12);
		} else if (gtk_wm.drag_mode == OXY_GTK_DRAG_SCROLL) {
			int viewport_height = oxy_GtkViewportHeight(gtk_wm.captured);
			int thumb_height = viewport_height * viewport_height /
				gtk_wm.captured->content_height;
			int travel;
			int position;
			if (thumb_height < 10) thumb_height = 10;
			travel = viewport_height - thumb_height;
			position = pointer_y - gtk_wm.captured->widget.position.y - 16 -
				thumb_height / 2;
			if (position < 0) position = 0;
			if (position > travel) position = travel;
			oxy_ScrollGtkWindowTo(gtk_wm.captured, travel > 0
				? oxy_GtkMaxScroll(gtk_wm.captured) * position / travel : 0);
		}
	}

	if (!pointer_down) {
		gtk_wm.captured = NULL;
		gtk_wm.drag_mode = OXY_GTK_DRAG_NONE;
	}

	if (gtk_wm.drag_mode == OXY_GTK_DRAG_MOVE) {
		oxy_SetMouseCursor(OXY_MOUSE_CURSOR_GRAB);
	} else if (gtk_wm.drag_mode == OXY_GTK_DRAG_RESIZE) {
		oxy_SetMouseCursor(OXY_MOUSE_CURSOR_GRAB);
	} else if (gtk_wm.drag_mode == OXY_GTK_DRAG_SCROLL) {
		oxy_SetMouseCursor(OXY_MOUSE_CURSOR_RESIZE_VERTICAL);
	} else if (hovered &&
		(oxy_GetGtkWindowControlAt(hovered, pointer_x, pointer_y) !=
			OXY_WINDOW_CONTROL_NONE ||
		 oxy_GtkScrollbarContains(hovered, pointer_x, pointer_y) ||
		 (hovered->hit_test_content && hovered->hit_test_content(
			hovered, hovered->hit_test_arg, pointer_x, pointer_y)))) {
		oxy_SetMouseCursor(OXY_MOUSE_CURSOR_POINTER);
	} else {
		oxy_SetMouseCursor(OXY_MOUSE_CURSOR_DEFAULT);
	}
	gtk_wm.pointer_down = pointer_down;
	return handled;
}

bool oxy_RenderGtkWindows(void)
{
	uint8_t index;
	uint8_t visible = 0;
	struct gtk_window_t *only_window = NULL;

	for (index = 0; index < gtk_wm.amount; index++) {
		if (gtk_wm.windows[index]->widget.state.visible) {
			visible++;
			only_window = gtk_wm.windows[index];
		}
	}

	if (gtk_wm.render_background) {
		gtk_wm.render_background(gtk_wm.background_arg);
	} else if (visible == 1) {
		oxy_RestoreGtkWindowBackground(only_window);
		oxy_ReleaseGtkWindowBackground(only_window);
		oxy_CaptureGtkWindowBackground(only_window);
	} else if (visible > 1) {
		return false;
	}

	for (index = 0; index < gtk_wm.amount; index++) {
		struct gtk_window_t *window = gtk_wm.windows[index];
		if (!window->widget.state.visible) continue;
		oxy_RenderGtkWindow(window);
		if (!window->minimized && window->render_content)
			window->render_content(window, window->render_arg);
		if (!window->minimized && oxy_GtkMaxScroll(window) > 0) {
			int viewport_height = oxy_GtkViewportHeight(window);
			int thumb_height = viewport_height * viewport_height /
				window->content_height;
			int thumb_y;
			int track_x = window->widget.position.x + window->widget.size.width - 9;
			if (thumb_height < 10) thumb_height = 10;
			thumb_y = window->widget.position.y + 16 + window->scroll_offset *
				(viewport_height - thumb_height) / oxy_GtkMaxScroll(window);
			gfx_SetColor(window->colors.color_a);
			oxy_RoundRectangle(track_x, window->widget.position.y + 16, 6,
				viewport_height, 0);
			gfx_SetColor(window->colors.text_fg);
			oxy_FillRoundRectangle(track_x + 1, thumb_y + 1, 4,
				thumb_height - 2, 0);
		}
	}
	gtk_wm.dirty = false;
	return true;
}

bool oxy_GtkWindowManagerNeedsRedraw(void)
{
	return gtk_wm.dirty;
}

void oxy_ReleaseGtkWindowBackground(struct gtk_window_t *window)
{
	if (!window) return;
	free(window->backing_left);
	free(window->backing_right);
	window->backing_left = NULL;
	window->backing_right = NULL;
	window->backing_valid = false;
}

bool oxy_CaptureGtkWindowBackground(struct gtk_window_t *window)
{
	if (!window || !window->widget.state.visible ||
		oxy_VisibleGtkWindowCount() != 1 || window->backing_valid) return false;

	int width = window->widget.size.width;
	int height = window->widget.size.height + 12;
	int left_width = width > 160 ? 160 : width;
	int right_width = width - left_width;
	if (height > LCD_HEIGHT - window->widget.position.y)
		height = LCD_HEIGHT - window->widget.position.y;

	window->backing_left = gfx_MallocSprite(left_width, height);
	if (right_width > 0)
		window->backing_right = gfx_MallocSprite(right_width, height);
	if (!window->backing_left || (right_width > 0 && !window->backing_right)) {
		oxy_ReleaseGtkWindowBackground(window);
		return false;
	}

	window->backing_position = window->widget.position;
	window->backing_size.width = width;
	window->backing_size.height = height;
	gfx_GetSprite(window->backing_left, window->backing_position.x,
				  window->backing_position.y);
	if (window->backing_right)
		gfx_GetSprite(window->backing_right,
					  window->backing_position.x + left_width,
					  window->backing_position.y);
	window->backing_valid = true;
	return true;
}

bool oxy_RestoreGtkWindowBackground(struct gtk_window_t *window)
{
	if (!window || !window->backing_valid || oxy_VisibleGtkWindowCount() != 1) return false;
	gfx_Sprite(window->backing_left, window->backing_position.x,
			   window->backing_position.y);
	if (window->backing_right)
		gfx_Sprite(window->backing_right,
				   window->backing_position.x + window->backing_left->width,
				   window->backing_position.y);
	return true;
}

static void oxy_SaveGtkWindowBounds(struct gtk_window_t *window)
{
	if (!window->minimized && !window->maximized) {
		window->restore_position = window->widget.position;
		window->restore_size = window->widget.size;
	}
}

void oxy_SetGtkWindowFeatures(struct gtk_window_t *window, bool closeable,
							  bool minimizable, bool maximizable, bool resizable)
{
	if (!window) return;
	window->closeable = closeable;
	window->minimizable = minimizable;
	window->maximizable = maximizable;
	window->resizable = resizable;
	gtk_wm.dirty = true;
}

void oxy_SetGtkWindowMinimumSize(struct gtk_window_t *window, int width, int height)
{
	if (!window) return;
	window->min_width = width > 1 ? width : 1;
	window->min_height = height > 1 ? height : 1;
}

bool oxy_ResizeGtkWindow(struct gtk_window_t *window, int width, int height)
{
	if (!window || !window->resizable || window->minimized || window->maximized) return false;
	if (width < window->min_width) width = window->min_width;
	if (height < window->min_height) height = window->min_height;
	if (width > LCD_WIDTH - window->widget.position.x) width = LCD_WIDTH - window->widget.position.x;
	if (height > LCD_HEIGHT - 12 - window->widget.position.y) height = LCD_HEIGHT - 12 - window->widget.position.y;
	window->widget.size.width = width;
	window->widget.size.height = height;
	window->restore_size = window->widget.size;
	gtk_wm.dirty = true;
	return true;
}

bool oxy_MinimizeGtkWindow(struct gtk_window_t *window)
{
	if (!window || !window->minimizable) return false;
	if (window->minimized) return oxy_RestoreGtkWindow(window);
	oxy_SaveGtkWindowBounds(window);
	window->minimized = true;
	window->maximized = false;
	gtk_wm.dirty = true;
	return true;
}

bool oxy_MaximizeGtkWindow(struct gtk_window_t *window)
{
	if (!window || !window->maximizable) return false;
	if (window->maximized) return oxy_RestoreGtkWindow(window);
	oxy_SaveGtkWindowBounds(window);
	window->widget.position.x = 0;
	window->widget.position.y = 0;
	window->widget.size.width = LCD_WIDTH;
	window->widget.size.height = LCD_HEIGHT - 12;
	window->minimized = false;
	window->maximized = true;
	gtk_wm.dirty = true;
	return true;
}

bool oxy_RestoreGtkWindow(struct gtk_window_t *window)
{
	if (!window || (!window->minimized && !window->maximized)) return false;
	window->widget.position = window->restore_position;
	window->widget.size = window->restore_size;
	window->minimized = false;
	window->maximized = false;
	gtk_wm.dirty = true;
	return true;
}

enum oxy_window_control_t oxy_GetGtkWindowControlAt(const struct gtk_window_t *window, int x, int y)
{
	if (!window || y < window->widget.position.y || y >= window->widget.position.y + 12) return OXY_WINDOW_CONTROL_NONE;
	int right = window->widget.position.x + window->widget.size.width;
	if (window->closeable && x >= right - 14 && x < right) return OXY_WINDOW_CONTROL_CLOSE;
	if (window->maximizable && x >= right - 28 && x < right - 14) return OXY_WINDOW_CONTROL_MAXIMIZE;
	if (window->minimizable && x >= right - 42 && x < right - 28) return OXY_WINDOW_CONTROL_MINIMIZE;
	return OXY_WINDOW_CONTROL_NONE;
}

bool oxy_ActivateGtkWindowControl(struct gtk_window_t *window, enum oxy_window_control_t control)
{
	switch (control) {
		case OXY_WINDOW_CONTROL_CLOSE:
			if (!window || !window->closeable) return false;
			window->widget.state.visible = false;
			gtk_wm.dirty = true;
			return true;
		case OXY_WINDOW_CONTROL_MINIMIZE:
			return oxy_MinimizeGtkWindow(window);
		case OXY_WINDOW_CONTROL_MAXIMIZE:
			return oxy_MaximizeGtkWindow(window);
		default:
			return false;
	}
}

void oxy_RenderGtkWindow(struct gtk_window_t *window)
{	
	if (window->widget.state.visible) {
		int x = window->widget.position.x;
		int y = window->widget.position.y;
		uint16_t width = window->widget.size.width;
		uint8_t height = window->widget.size.height;
		
		/* Title Area */
		gfx_SetColor(window->colors.color_a);
		oxy_FillRoundRectangle(x, y, width, window->minimized ? 12 : height + 12, 0);
		
		gfx_SetTextBGColor(window->colors.color_a);
		gfx_SetTextFGColor(window->colors.text_fg);
		gfx_SetTextTransparentColor(0);
		gfx_PrintStringXY(window->title, x + (width - gfx_GetStringWidth(window->title)) / 2, y + 3);
		if (window->minimizable) gfx_PrintStringXY("_", x + width - 39, y + 2);
		if (window->maximizable) gfx_PrintStringXY(window->maximized ? "o" : "+", x + width - 25, y + 2);
		if (window->closeable) gfx_PrintStringXY("X", x + width - 11, y + 2);
		
		/* Widget Area */
		if (!window->minimized) {
			gfx_SetColor(window->colors.color_b);
			oxy_FillRoundRectangle(x + 1, y + 13, width - 2, height - 2, 0);
			if (window->resizable && !window->maximized) {
				gfx_SetColor(window->colors.color_a);
				gfx_HorizLine(x + width - 8, y + height + 8, 6);
				gfx_HorizLine(x + width - 5, y + height + 5, 3);
			}
		}
	}
}

void oxy_CenteredWindow(char *title, int width, int height)
{
	struct gtk_window_t *window = oxy_CreateWindow(
		title, 160 - width / 2, 120 - (height + 13) / 2, width, height
	);

	if (window == NULL) return;
	oxy_RenderGtkWindow(window);
	oxy_DestroyWindow(window);
}

// Window Colors
void oxy_SetWindowColor(struct gtk_window_t *window, uint8_t boarder_color, uint8_t center_color)
{
	window->colors.color_a = boarder_color;
	window->colors.color_b = center_color;
	gtk_wm.dirty = true;
}

void oxy_SetWindowTextColor(struct gtk_window_t *window, uint8_t text_fg, uint8_t text_bg)
{
	window->colors.text_fg = text_fg;
	window->colors.text_bg = text_bg;
	gtk_wm.dirty = true;
}

// 	Message & Dialogue
void oxy_Message(char *title, char *message) 
{	
	const int width = 100;
	const int height = 80;
	
    uint8_t numlines = 7;
		
    const uint8_t titlewidth = 12;
    uint8_t padding = 3;
	
	uint8_t xprint = 160 - (100 + 2 * padding) / 2;
    uint8_t yprint;
    
	if (title != NULL) {
		yprint = 120 - (height + 13) / 2;
		oxy_CenteredWindow(title, width, height);
		oxy_PrintWordWrap(message, xprint + 2 * padding, yprint + titlewidth + 1, width, numlines, 0);
	}else{
		yprint = 120 - (numlines * 3 + titlewidth + 2 * padding) / 2;
		oxy_FillRoundRectangle(xprint, yprint, width, height, 0);
		oxy_PrintWordWrap(message, xprint + 2 * padding, yprint + titlewidth + padding + 1, width, numlines, 0);
	}
	
	gfx_Blit(1);
	while (!os_GetCSC());	
}

void oxy_PrintWordWrap(const char *text, uint24_t x, uint8_t y, int width,
					   uint8_t max_lines, uint8_t init_line)
{
	uint8_t logical_line = 0;
	uint8_t printed_lines = 0;
	char line[64];

	if (!text || width <= 0 || max_lines == 0) return;
	if (width > LCD_WIDTH - (int)x) width = LCD_WIDTH - x;
	while (*text && printed_lines < max_lines) {
		size_t length = 0;
		size_t break_at = 0;
		int line_width = 0;
		bool width_exceeded = false;

		while (text[length] && text[length] != '\n' &&
			   length < sizeof(line) - 1) {
			int character_width = gfx_GetCharWidth(text[length]);
			if (line_width + character_width > width && length > 0) {
				width_exceeded = true;
				break;
			}
			line_width += character_width;
			if (text[length] == ' ') break_at = length;
			length++;
		}
		if (width_exceeded && break_at > 0) length = break_at;
		if (length == 0 && *text != '\n') length = 1;
		memcpy(line, text, length);
		while (length > 0 && line[length - 1] == ' ') length--;
		line[length] = '\0';
		if (logical_line >= init_line) {
			gfx_PrintStringXY(line, x, y + printed_lines * 9);
			printed_lines++;
		}
		logical_line++;
		text += length;
		while (*text == ' ') text++;
		if (*text == '\n') text++;
	}
}
