/**
 * @file oxy_gtk.h
 * @brief Declares Oxygen's window manager, window widgets, and form helpers.
 */
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
struct oxy_label_t;
struct oxy_widget_t;

/**
 * @brief Background renderer used before windows are composited.
 */
typedef void (*oxy_gtk_background_callback_t)(void *arg);

/**
 * @brief Window content renderer callback.
 */
typedef void (*oxy_gtk_window_callback_t)(struct gtk_window_t *window, void *arg);

/**
 * @brief Hit-test callback used for custom window content.
 */
typedef bool (*oxy_gtk_hit_test_callback_t)(struct gtk_window_t *window,
											 void *arg, int x, int y);

/**
 * @brief Built-in window controls exposed by the title bar and resize handle.
 */
enum oxy_window_control_t {
	OXY_WINDOW_CONTROL_NONE,
	OXY_WINDOW_CONTROL_MINIMIZE,
	OXY_WINDOW_CONTROL_MAXIMIZE,
	OXY_WINDOW_CONTROL_CLOSE,
	OXY_WINDOW_CONTROL_RESIZE
};

/**
 * @brief Stores the colors used by GTK-style widgets and windows.
 */
struct gtk_color_t {
	uint8_t text_bg;
	uint8_t text_fg;
	
	uint8_t color_a;
	uint8_t color_b;
};

/**
 * @brief Tracks common widget visibility and click state.
 */
struct gtk_state_t {
	bool visible;
	bool clicked;
	bool clickable;
};

/**
 * @brief Stores a widget or window position.
 */
struct gtk_position_t {
	int x;
    int y;
};

/**
 * @brief Stores width and height values.
 */
struct gtk_sizeinfo_t {
    int width;
    int height;
};

/**
 * @brief Base structure for displayable GTK-style objects.
 */
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

/**
 * @brief Represents a managed Oxygen window.
 */
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
	oxy_gtk_hit_test_callback_t hit_test_content;
	void *hit_test_arg;
	int content_height;
	int scroll_offset;
};

/**
 * @brief Global state for the Oxygen window manager.
 */
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
 * @brief Associates a form label with its interactive control.
 */
struct oxy_gtk_form_row_t {
	struct oxy_label_t *label;
	struct oxy_widget_t *control;
};

/**
 * @brief Tracks selection and layout state for a form rendered inside a window.
 */
struct oxy_gtk_form_t {
	struct gtk_window_t *window;
	struct oxy_gtk_form_row_t *rows;
	uint8_t row_count;
	uint8_t selected_row;
	uint8_t selection_color;
	struct oxy_widget_t *action_button;
};


/**
 * @brief Creates a managed window.
 * @param title Window title, up to eight characters plus terminator.
 * @param x Initial X coordinate.
 * @param y Initial Y coordinate.
 * @param width Initial window width.
 * @param height Initial window height.
 * @return Pointer to the created window.
 */
struct gtk_window_t* oxy_CreateWindow(char title[9], int x, uint8_t y, int width, int height);

/**
 * @brief Destroys a managed window and releases any resources it owns.
 * @param window Window to destroy.
 */
void oxy_DestroyWindow(struct gtk_window_t *window);

/**
 * @brief Updates a window's state and content interactions.
 * @param window Window to update.
 */
void oxy_UpdateGtkWindow(struct gtk_window_t *window);

/**
 * @brief Renders a single window.
 * @param window Window to render.
 */
void oxy_RenderGtkWindow(struct gtk_window_t *window);

/**
 * @brief Sets the callback used to render a window's content area.
 * @param window Target window.
 * @param callback Content renderer.
 * @param arg Opaque renderer argument.
 */
void oxy_SetGtkWindowRenderer(struct gtk_window_t *window,
							  oxy_gtk_window_callback_t callback, void *arg);

/**
 * @brief Sets the hit-test callback for a window's content area.
 * @param window Target window.
 * @param callback Hit-test callback.
 * @param arg Opaque callback argument.
 */
void oxy_SetGtkWindowHitTest(struct gtk_window_t *window,
							 oxy_gtk_hit_test_callback_t callback, void *arg);

/**
 * @brief Sets the total scrollable content height for a window.
 * @param window Target window.
 * @param height Content height in pixels.
 */
void oxy_SetGtkWindowContentHeight(struct gtk_window_t *window, int height);

/**
 * @brief Returns the current content scroll offset for a window.
 * @param window Window to query.
 * @return Current scroll offset in pixels.
 */
int oxy_GetGtkWindowScrollOffset(const struct gtk_window_t *window);

/**
 * @brief Scrolls a window's content to a specific offset.
 * @param window Target window.
 * @param offset Requested scroll offset.
 * @return `true` if the offset changed.
 */
bool oxy_ScrollGtkWindowTo(struct gtk_window_t *window, int offset);

/**
 * @brief Initializes a form descriptor for a window.
 * @param form Form state to initialize.
 * @param window Window that owns the form.
 * @param rows Form rows to render.
 * @param row_count Number of rows.
 * @param action_button Optional action button rendered after the rows.
 */
void oxy_InitGtkForm(struct oxy_gtk_form_t *form, struct gtk_window_t *window,
					 struct oxy_gtk_form_row_t *rows, uint8_t row_count,
					 struct oxy_widget_t *action_button);

/**
 * @brief Renders a form inside a window's content area.
 * @param window Window being rendered.
 * @param arg Pointer to the form descriptor.
 */
void oxy_RenderGtkForm(struct gtk_window_t *window, void *arg);

/**
 * @brief Performs hit testing for widgets contained in a form.
 * @param window Window receiving pointer input.
 * @param arg Pointer to the form descriptor.
 * @param x Pointer X coordinate relative to the window.
 * @param y Pointer Y coordinate relative to the window.
 * @return `true` if the hit was handled by the form.
 */
bool oxy_GtkFormHitTest(struct gtk_window_t *window, void *arg, int x, int y);

/**
 * @brief Returns the form row at the given local coordinates.
 * @param form Form descriptor to query.
 * @param x Local X coordinate.
 * @param y Local Y coordinate.
 * @return Row index, or a negative value when no row matches.
 */
int oxy_GtkFormRowAt(const struct oxy_gtk_form_t *form, int x, int y);

/**
 * @brief Changes the selected form row.
 * @param form Form descriptor to update.
 * @param row Row index to select.
 */
void oxy_SetGtkFormSelection(struct oxy_gtk_form_t *form, uint8_t row);

/**
 * @brief Sets the highlight color used for the selected form row.
 * @param form Form descriptor to update.
 * @param color Palette index used for selection.
 */
void oxy_SetGtkFormSelectionColor(struct oxy_gtk_form_t *form, uint8_t color);

/**
 * @brief Scrolls the window until the selected form row is visible.
 * @param form Form descriptor to update.
 * @return `true` if scrolling changed.
 */
bool oxy_EnsureGtkFormSelectionVisible(struct oxy_gtk_form_t *form);

/**
 * @brief Sets the renderer used for the desktop background behind all windows.
 * @param callback Background renderer.
 * @param arg Opaque renderer argument.
 */
void oxy_SetGtkWindowManagerBackground(oxy_gtk_background_callback_t callback,
									 void *arg);

/**
 * @brief Returns the currently focused window.
 * @return Focused window, or `NULL` when no window is focused.
 */
struct gtk_window_t *oxy_GetFocusedGtkWindow(void);

/**
 * @brief Returns the topmost window containing a screen coordinate.
 * @param x Screen X coordinate.
 * @param y Screen Y coordinate.
 * @return Window at that position, or `NULL` when none matches.
 */
struct gtk_window_t *oxy_GetGtkWindowAt(int x, int y);

/**
 * @brief Moves a window to the front of the z-order.
 * @param window Window to raise.
 * @return `true` if the z-order changed.
 */
bool oxy_BringGtkWindowToFront(struct gtk_window_t *window);

/**
 * @brief Moves a window to a new position.
 * @param window Window to move.
 * @param x New X coordinate.
 * @param y New Y coordinate.
 * @return `true` if the window moved.
 */
bool oxy_MoveGtkWindow(struct gtk_window_t *window, int x, int y);

/**
 * @brief Updates global window-manager interactions.
 * @param pointer_x Current pointer X coordinate.
 * @param pointer_y Current pointer Y coordinate.
 * @param pointer_down Whether the primary pointer action is held.
 * @return `true` if the update changed visible state.
 */
bool oxy_UpdateGtkWindowManager(int pointer_x, int pointer_y, bool pointer_down);

/**
 * @brief Renders every managed window.
 * @return `true` if any windows were rendered.
 */
bool oxy_RenderGtkWindows(void);

/**
 * @brief Returns whether the window manager needs a redraw.
 * @return `true` if a redraw is pending.
 */
bool oxy_GtkWindowManagerNeedsRedraw(void);

/**
 * @brief Captures the background pixels under a window.
 * @param window Window whose background should be captured.
 * @return `true` if the background capture succeeded.
 */
bool oxy_CaptureGtkWindowBackground(struct gtk_window_t *window);

/**
 * @brief Restores previously captured background pixels for a window.
 * @param window Window whose background should be restored.
 * @return `true` if the background was restored.
 */
bool oxy_RestoreGtkWindowBackground(struct gtk_window_t *window);

/**
 * @brief Releases any captured background buffers for a window.
 * @param window Window to clean up.
 */
void oxy_ReleaseGtkWindowBackground(struct gtk_window_t *window);

/**
 * @brief Enables or disables built-in window controls.
 * @param window Window to configure.
 * @param closeable Whether the close control is enabled.
 * @param minimizable Whether the minimize control is enabled.
 * @param maximizable Whether the maximize control is enabled.
 * @param resizable Whether resizing is enabled.
 */
void oxy_SetGtkWindowFeatures(struct gtk_window_t *window, bool closeable,
							  bool minimizable, bool maximizable, bool resizable);

/**
 * @brief Sets the minimum size allowed when resizing a window.
 * @param window Window to configure.
 * @param width Minimum width.
 * @param height Minimum height.
 */
void oxy_SetGtkWindowMinimumSize(struct gtk_window_t *window, int width, int height);

/**
 * @brief Resizes a window.
 * @param window Window to resize.
 * @param width Requested width.
 * @param height Requested height.
 * @return `true` if the size changed.
 */
bool oxy_ResizeGtkWindow(struct gtk_window_t *window, int width, int height);

/**
 * @brief Minimizes a window.
 * @param window Window to minimize.
 * @return `true` if the window changed state.
 */
bool oxy_MinimizeGtkWindow(struct gtk_window_t *window);

/**
 * @brief Maximizes a window.
 * @param window Window to maximize.
 * @return `true` if the window changed state.
 */
bool oxy_MaximizeGtkWindow(struct gtk_window_t *window);

/**
 * @brief Restores a minimized or maximized window.
 * @param window Window to restore.
 * @return `true` if the window changed state.
 */
bool oxy_RestoreGtkWindow(struct gtk_window_t *window);

/**
 * @brief Returns the window control under a given screen coordinate.
 * @param window Window to test.
 * @param x Screen X coordinate.
 * @param y Screen Y coordinate.
 * @return Matching control, or OXY_WINDOW_CONTROL_NONE.
 */
enum oxy_window_control_t oxy_GetGtkWindowControlAt(const struct gtk_window_t *window,
												 int x, int y);

/**
 * @brief Activates one of the built-in window controls.
 * @param window Window to update.
 * @param control Control to activate.
 * @return `true` if the control changed window state.
 */
bool oxy_ActivateGtkWindowControl(struct gtk_window_t *window, enum oxy_window_control_t control);

/**
 * @brief Creates a window centered on screen.
 * @param title Window title.
 * @param width Window width.
 * @param height Window height.
 */
void oxy_CenteredWindow(char *title, int width, int height);

/**
 * @brief Sets the primary window colors.
 * @param window Window to update.
 * @param boarder_color Border color index.
 * @param center_color Fill color index.
 */
void oxy_SetWindowColor(struct gtk_window_t *window, uint8_t boarder_color, uint8_t center_color);

/**
 * @brief Sets the title text colors for a window.
 * @param window Window to update.
 * @param text_fg Foreground color index.
 * @param text_bg Background color index.
 */
void oxy_SetWindowTextColor(struct gtk_window_t *window, uint8_t text_fg, uint8_t text_bg);

/**
 * @brief Displays a simple message dialog.
 * @param title Dialog title.
 * @param message Dialog message text.
 */
void oxy_Message(char *title, char *message);

/**
 * @brief Prints wrapped text inside a fixed-width region.
 * @param text Text to render.
 * @param x X coordinate.
 * @param y Y coordinate.
 * @param width Wrap width in pixels.
 * @param max_lines Maximum number of lines to draw.
 * @param init_line First logical line to render.
 */
void oxy_PrintWordWrap(const char *text, uint24_t x, uint8_t y, int width,
					   uint8_t max_lines, uint8_t init_line);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __OXY_GTK_H__ */