/**
 * @file oxy_mouse.h
 * @brief Declares pointer state, hover regions, and cursor rendering helpers.
 */
#ifndef OXY_MOUSE_H
#define OXY_MOUSE_H

#include <tice.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @brief Cursor styles supported by the Oxygen pointer renderer.
 */
enum oxy_mouse_cursor_t {
	OXY_MOUSE_CURSOR_DEFAULT,
	OXY_MOUSE_CURSOR_POINTER,
	OXY_MOUSE_CURSOR_GRAB,
	OXY_MOUSE_CURSOR_RESIZE_VERTICAL,
	OXY_MOUSE_CURSOR_RESIZE_HORIZONTAL
};

/**
 * @brief Holds the current pointer state and hover bookkeeping.
 */
struct oxy_mouse_t {
	int x;
	int y;
	
	uint8_t speed;
	
	int scroll_X;
	int scroll_Y;
	
	uint8_t hover_amount; 
	int clicked_index;
	int captured_index;
	enum oxy_mouse_cursor_t cursor;
	bool cursor_override;
};
extern struct oxy_mouse_t oxy_mouse;

/**
 * @brief Describes a hover target used for hit testing and pointer actions.
 */
struct oxy_detect_t{
	uint16_t x;
	uint8_t y; 
	
	uint16_t w; 
	uint8_t h;
	
	uint8_t data; // any number data you want to use.
	enum oxy_mouse_cursor_t hover_cursor;
	enum oxy_mouse_cursor_t active_cursor;
	void (*right_click)(void *); // function for clicking.
	void *right_arg; // arguments for "click"
	
	void (*left_click)(void *); // function for clicking.
	void *left_arg; // arguments for "click"
	
	char *description;
};
extern struct oxy_detect_t *oxy_detect;

/**
 * @brief Initializes the pointer state and hover registry.
 */
void oxy_InitMouse(void);

/**
 * @brief Resets pointer scrolling and click tracking state.
 */
void oxy_ResetMouse(void);


/**
 * @brief Registers a new hover region.
 * @param x Hover region X position.
 * @param y Hover region Y position.
 * @param w Hover region width.
 * @param h Hover region height.
 * @return Hover-region index.
 */
uint8_t oxy_AddHover(uint16_t x, uint8_t y, uint16_t w, uint8_t h);

/**
 * @brief Updates the bounds of an existing hover region.
 * @param index Hover-region index.
 * @param x New X position.
 * @param y New Y position.
 * @param width New width.
 * @param height New height.
 */
void oxy_SetHoverBounds(uint8_t index, uint16_t x, uint8_t y,
						uint16_t width, uint8_t height);

/**
 * @brief Sets the hover and active cursor styles for a hover region.
 * @param index Hover-region index.
 * @param hover_cursor Cursor shown while hovering.
 * @param active_cursor Cursor shown while pressed or captured.
 */
void oxy_SetHoverCursors(uint8_t index, enum oxy_mouse_cursor_t hover_cursor,
						 enum oxy_mouse_cursor_t active_cursor);

/**
 * @brief Removes a hover region.
 * @param index Hover-region index returned by oxy_AddHover.
 * @return `true` if the region was removed.
 */
bool oxy_RemoveHover(uint8_t index);

/**
 * @brief Removes all hover regions.
 */
void oxy_RemoveAllHover(void);

/**
 * @brief Removes every hover region with an index below the given value.
 */ 
void oxy_RemoveAllBelow(uint8_t index);

/**
 * @brief Removes every hover region with an index above the given value.
 */ 
void oxy_RemoveAllAbove(uint8_t index);

/**
 * @brief Attaches a description string to a hover region.
 * @param text Hover description.
 * @param index Hover-region index returned by oxy_AddHover.
 * @return `true` if the description was stored successfully.
 */
bool oxy_SetHoverDescription(char text[], uint8_t index);

/**
 * @brief Returns the index of the most recently clicked hover region.
 * @return Hover index, or a negative value when nothing is clicked.
 */
int oxy_ReturnClickedIndex(void);

/**
 * @brief Sets the secondary-click callback for a hover region.
 * @param function Callback with signature `void callback(void *arg)`.
 * @param arg Callback argument.
 * @param index Hover-region index.
 */
void oxy_SetRightClick(void *function, void *arg, uint8_t index);

/**
 * @brief Sets the primary-click callback for a hover region.
 * @param function Callback with signature `void callback(void *arg)`.
 * @param arg Callback argument.
 * @param index Hover-region index.
 */
void oxy_SetLeftClick(void *function, void *arg, uint8_t index);

/**
 * @brief Sets the pointer movement speed.
 * @param speed New speed value.
 */
void oxy_SetMouseSpeed(uint8_t speed);

/**
 * @brief Selects the cursor sprite used to render the pointer.
 * @param cursor Cursor style to activate.
 */
void oxy_SetMouseCursor(enum oxy_mouse_cursor_t cursor);

/**
 * @brief Renders the pointer.
 */
void oxy_RenderMouse(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif