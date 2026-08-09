/**
 * @file oxy_gui.h
 * @brief Declares GUI helpers such as text input, color picking, and status widgets.
 */
#ifndef OXY_GUI_H
#define OXY_GUI_H

#include <tice.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @brief Tracks the current state of an interactive string input prompt.
 */
struct oxy_stringinput_t {
    int charsamount; 
    char *text; 
    uint8_t type;
	bool forced_exit;
	uint8_t max_char;
};
extern struct oxy_stringinput_t oxy_stringinput;

// String input.
/**
 * @brief Prompts the user for a string.
 * @param title Prompt text displayed before the input field.
 * @param x X coordinate.
 * @param y Y coordinate.
 * @param maxchar Maximum input length.
 * @return Pointer to the entered string.
 */
char* oxy_StringInput(const char title[], uint16_t x, uint8_t y, uint8_t maxchar);


// Others.
/**
 * @brief Draws an interactive color picker.
 * @param cur_select Preselected color, from 0 to 255.
 * @param x X coordinate.
 * @param y Y coordinate.
 * @return Selected palette index.
 */
uint8_t oxy_ColorPicker(uint8_t cur_select, uint16_t x, uint8_t y);

/**
 * @brief Prints the current time.
 * @param x X coordinate.
 * @param y Y coordinate.
 */
void oxy_PrintTime(uint16_t x, uint8_t y);

/**
 * @brief Prints the current date.
 * @param x X coordinate.
 * @param y Y coordinate.
 */
void oxy_PrintDate(uint16_t x, uint8_t y);

/**
 * @brief Prints the battery-status icon.
 * @param x X coordinate.
 * @param y Y coordinate.
 */
void oxy_PrintBatteryStatus(uint16_t x, uint8_t y);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __OXY_GUI_H__ */