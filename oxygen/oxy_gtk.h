#ifndef OXY_GTK_H
#define OXY_GTK_H

#include <tice.h>
#include <graphx.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define GTK_NUM_TYPE		0
#define GTK_TEXT_TYPE		1
#define GTK_SPRITE_TYPE		2
#define GTK_BUTTON_TYPE		3
#define GTK_WINDOW_TYPE		4
#define GTK_DIVIDER_TYPE	5
#define OXY_GTK_MAX_WINDOWS 8

struct gtk_window_t;

typedef void (*oxy_gtk_background_callback_t)(void *arg);
typedef void (*oxy_gtk_window_callback_t)(struct gtk_window_t *window, void *arg);

enum oxy_window_control_t {
	OXY_WINDOW_CONTROL_NONE,
	OXY_WINDOW_CONTROL_MINIMIZE,
	OXY_WINDOW_CONTROL_MAXIMIZE,
	OXY_WINDOW_CONTROL_CLOSE,
	OXY_WINDOW_CONTROL_RESIZE
};

/* Color of gtk objects */
struct gtk_color_t {
	uint8_t text_bg;
	uint8_t text_fg;
	
	uint8_t color_a;
	uint8_t color_b;
};

/* State of an object "derived from gtk" */
struct gtk_state_t {
	bool visible;
	bool clicked;
	bool clickable;
};

/* Array that contains Position and offsets */
struct gtk_position_t {
	int x;
    int y;
};

/* array that contains width and height data */
struct gtk_sizeinfo_t {
    int width;
    int height;
};

/* The widget is the base of the tree for displayable objects. */
struct gtk_widget_t {
	uint8_t type;
	
	struct gtk_color_t color;
	struct gtk_position_t position;
	struct gtk_sizeinfo_t size;
	struct gtk_state_t state;
	
	void (*update)(struct gtk_widget_t *);
	void (*render)(struct gtk_widget_t *);
    
	struct gtk_widget_t **child;
};

/* Window Array */
struct gtk_window_t {
	char title[9];
	char wm_class[9];
	
	struct gtk_widget_t widget;	
	struct gtk_color_t colors;
	struct gtk_position_t restore_position;
	struct gtk_sizeinfo_t restore_size;
	int min_width;
	int min_height;
	bool closeable;
	bool minimizable;
	bool maximizable;
	bool resizable;
	bool minimized;
	bool maximized;
	gfx_sprite_t *backing_left;
	gfx_sprite_t *backing_right;
	struct gtk_position_t backing_position;
	struct gtk_sizeinfo_t backing_size;
	bool backing_valid;
	oxy_gtk_window_callback_t render_content;
	void *render_arg;
};

/* Window management system */
struct gtk_wm_t {
	uint8_t amount;
	struct gtk_window_t *windows[OXY_GTK_MAX_WINDOWS];
	struct gtk_window_t *focused;
	struct gtk_window_t *captured;
	oxy_gtk_background_callback_t render_background;
	void *background_arg;
	int drag_offset_x;
	int drag_offset_y;
	uint8_t drag_mode;
	bool pointer_down;
	bool dirty;
};
extern struct gtk_wm_t gtk_wm;


/**
 * Gets string input from user.
 * @param title Input text (text displayed before input).
 * @param x X coordinate
 * @param y Y coordinate
 * @param width Max input characters
 * @param height Max input characters
 * @returns gtk_window_t* Return users input
 */
struct gtk_window_t* oxy_CreateWindow(char title[9], int x, uint8_t y, int width, int height);

void oxy_DestroyWindow(struct gtk_window_t *window);

void oxy_UpdateGtkWindow(struct gtk_window_t *window);

void oxy_RenderGtkWindow(struct gtk_window_t *window);

void oxy_SetGtkWindowRenderer(struct gtk_window_t *window,
							  oxy_gtk_window_callback_t callback, void *arg);

void oxy_SetGtkWindowManagerBackground(oxy_gtk_background_callback_t callback,
									 void *arg);

struct gtk_window_t *oxy_GetFocusedGtkWindow(void);

struct gtk_window_t *oxy_GetGtkWindowAt(int x, int y);

bool oxy_BringGtkWindowToFront(struct gtk_window_t *window);

bool oxy_MoveGtkWindow(struct gtk_window_t *window, int x, int y);

bool oxy_UpdateGtkWindowManager(int pointer_x, int pointer_y, bool pointer_down);

bool oxy_RenderGtkWindows(void);

bool oxy_GtkWindowManagerNeedsRedraw(void);

bool oxy_CaptureGtkWindowBackground(struct gtk_window_t *window);

bool oxy_RestoreGtkWindowBackground(struct gtk_window_t *window);

void oxy_ReleaseGtkWindowBackground(struct gtk_window_t *window);

void oxy_SetGtkWindowFeatures(struct gtk_window_t *window, bool closeable,
							  bool minimizable, bool maximizable, bool resizable);

void oxy_SetGtkWindowMinimumSize(struct gtk_window_t *window, int width, int height);

bool oxy_ResizeGtkWindow(struct gtk_window_t *window, int width, int height);

bool oxy_MinimizeGtkWindow(struct gtk_window_t *window);

bool oxy_MaximizeGtkWindow(struct gtk_window_t *window);

bool oxy_RestoreGtkWindow(struct gtk_window_t *window);

enum oxy_window_control_t oxy_GetGtkWindowControlAt(const struct gtk_window_t *window,
												 int x, int y);

bool oxy_ActivateGtkWindowControl(struct gtk_window_t *window, enum oxy_window_control_t control);

void oxy_CenteredWindow(char *title, int width, int height);

void oxy_SetWindowColor(struct gtk_window_t *window, uint8_t boarder_color, uint8_t center_color);

void oxy_SetWindowTextColor(struct gtk_window_t *window, uint8_t text_fg, uint8_t text_bg);

void oxy_Message(char *title, char *message);

void oxy_PrintWordWrap(char *text, uint24_t x, uint8_t y, int width, uint8_t max_lines, uint8_t init_line);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __OXY_GTK_H__ */