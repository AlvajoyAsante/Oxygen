#include "oxy_gfx.h"
#include "oxy_gtk.h"

#include <tice.h>
#include <debug.h>
#include <graphx.h>
#include <keypadc.h>
#include <string.h>
#include <ctype.h>
#include <fileioc.h>
#include <stdlib.h>

uint8_t _getWordWidth(char *word);
bool _iscntrl(int c);

struct gtk_wm_t gtk_wm;

enum {
	OXY_GTK_DRAG_NONE,
	OXY_GTK_DRAG_MOVE,
	OXY_GTK_DRAG_RESIZE
};

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

	if (pressed) {
		struct gtk_window_t *window = oxy_GetGtkWindowAt(pointer_x, pointer_y);
		gtk_wm.captured = NULL;
		gtk_wm.drag_mode = OXY_GTK_DRAG_NONE;
		if (window) {
			enum oxy_window_control_t control;
			oxy_BringGtkWindowToFront(window);
			control = oxy_GetGtkWindowControlAt(window, pointer_x, pointer_y);
			if (control != OXY_WINDOW_CONTROL_NONE) {
				oxy_ActivateGtkWindowControl(window, control);
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
		}
	}

	if (!pointer_down) {
		gtk_wm.captured = NULL;
		gtk_wm.drag_mode = OXY_GTK_DRAG_NONE;
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

/**
 * Implemented from "Captain-Calc/textioc"
 * This function works on the assumption that the longest word is <256 characters long
 */
uint8_t _getWordWidth(char *word) 
{
	char *c = word;
	uint8_t width = 0;

	while (!isspace(*c) && !_iscntrl(*c) && width < 240) 
		width += gfx_GetCharWidth(*c++);
	
	return width;
}

bool _iscntrl(int c) 
{
	if (c == '\0' || c == '\n' || c == '\t') {
		return 1;
	}else return 0;
}

void oxy_PrintWordWrap(char *text, uint24_t x, uint8_t y, int width, uint8_t max_lines, uint8_t init_line)
{
	char *curr_char = text;
	uint24_t curr_line_width = 0;
	int word_width = 0;
	uint8_t curr_line_num = init_line;
	uint8_t i;
	
	// Debugging
	gfx_SetColor(224);
	gfx_FillRectangle(x + width, 0, 2, LCD_HEIGHT);
	
	if (gfx_GetStringWidth(text) <= (unsigned int)width) {
		gfx_SetTextXY(x, y);
		gfx_PrintString(text);
		return;
	};
	
	// If the width is way bigger than the LCD_Width
	if (width > LCD_WIDTH) width = LCD_WIDTH;
	
	dbg_sprintf(dbgout, "Current Line: 1 -------------\n");
	
	for ( ; ; ) {

		if (curr_line_num > max_lines || y > LCD_HEIGHT)
			return;

		gfx_SetTextXY(x, y + (curr_line_num - init_line) * 4);
		curr_line_width = 0;

		while (*curr_char != '\0') {

			// Get the width of the next word
			word_width = _getWordWidth(curr_char);
			dbg_sprintf(dbgout, "word_width = %d | Word: ", word_width);

			// If there is room on the current line for the word, print the word
			// else, start a new line
			if (curr_line_width + word_width < (unsigned int)width) {
				curr_line_width += word_width;
				while (!isspace(*curr_char) && !iscntrl(*curr_char)) {
					dbg_sprintf(dbgout, "%c", *curr_char);
					gfx_PrintChar(*curr_char++);
				};
				dbg_sprintf(dbgout, "\n");
			} else {
				dbg_sprintf(dbgout, "Cannot put word on current line. Breaking...\n");
				goto startLine;
			};

			if (*curr_char == '\0') return;

			switch (*curr_char++) {
				case '\t':
					dbg_sprintf(dbgout, "Handling horizontal tab...\n");
					if (curr_line_width + (4 * gfx_GetCharWidth(' ')) < (unsigned int)width) {
						for (i = 1; i < 4; i++) {
							gfx_PrintChar(' ');
							curr_line_width += gfx_GetCharWidth(' ');
						};
					} else {
						dbg_sprintf(dbgout, "Cannot put tab on current line. Breaking...\n");
						goto startLine;
					};
					break;
				
				case '\n':
					dbg_sprintf(dbgout, "Handling newline...\n");
					goto startLine;
				
				case '\v':
					dbg_sprintf(dbgout, "Handling vertical tab...\n");
					goto startLine;
				
				case ' ':
					dbg_sprintf(dbgout, "Handling space...\n");
					gfx_PrintChar(*(curr_char - 1));
					curr_line_width += gfx_GetCharWidth(' ');
					break;
			};

		};

		startLine:
		
		curr_line_num++;
		dbg_sprintf(dbgout, "Line: %d -------------\n", curr_line_num);
	};
}
