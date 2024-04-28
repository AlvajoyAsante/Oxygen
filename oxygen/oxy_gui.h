/**
 * @file oxy_gui.h
 * @brief This file contains simple GUI elements
 *
 * @author Alvajoy "Alvajoy123" Asante
 * @version 1.0
 * @copyright Copyright (c) 2018 - 2024
 *
 *      _/_/    _/      _/  _/      _/    _/_/_/  _/_/_/_/  _/      _/
 *   _/    _/    _/  _/      _/  _/    _/        _/        _/_/    _/
 *  _/    _/      _/          _/      _/  _/_/  _/_/_/    _/  _/  _/
 * _/    _/    _/  _/        _/      _/    _/  _/        _/    _/_/
 *  _/_/    _/      _/      _/        _/_/_/  _/_/_/_/  _/      _/
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef OXY_GUI_H
#define OXY_GUI_H

#include <tice.h>
#include "oxy_widget.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

	/**
	 * @brief Structure for the string input.
	 */
	struct oxy_stringinput_t
	{
		int charsamount;
		char *text;
		uint8_t type;
		uint8_t max_char;
		bool forced_exit;

		// History
		bool enable_history;
		char *history[256];
		uint8_t history_pos;
		uint8_t history_size;

		// Autofill
		bool enable_autofill;
		char *autofill[500];
		int autofill_size;

		// Functions Check
		bool enable_function_check;
		bool (*function_check)(void *);
		bool function_check_result;
	};
	extern struct oxy_stringinput_t oxy_stringinput;

	/**
	 * @brief Renders a message on the screen.
	 *
	 * This function displays a message on the screen with the given title.
	 *
	 * @param title Title of the message.
	 * @param message The message to be displayed.
	 */
	void oxy_Message(const char *title, char *message);
	
	/**
	 * @brief Resets the string input history.
	 * 
	 * @return ** void 
	 */
	void oxy_ResetStringInputHistory(void);

	/**
	 * @brief Adds a string to the string input history.
	 * 
	 * @param input The string to add to the history.
	 */
	void oxy_StringInput_AddToHistory(const char *input);

	/**
	 * @brief Resets the string input autofill.
	 * 
	 * @return ** void 
	 */
	void oxy_ResetStringInputAutofill(void);
	
	/**
	 * @brief Adds a string to the string input autofill.
	 * 
	 * @param input The string to add to the autofill.
	 */
	void oxy_StringInput_AddToAutofill(const char *input);

	/**
	 * @brief Renders a message with buttons.
	 *
	 * This function displays a message on the screen with the given title and a list of buttons.
	 *
	 * @param title Title of the message.
	 * @param message The message to be displayed.
	 * @param buttons Array of buttons to be displayed.
	 */
	void oxy_Dialogue(const char *title, char *message, struct oxy_button_t **buttons);

	// String input.
	/**
	 * @brief Gets string input from the user.
	 *
	 * This function prompts the user to enter a string of input with the given title.
	 *
	 * @param title Input text (text displayed before input).
	 * @param x The x-coordinate of the input box.
	 * @param y The y-coordinate of the input box.
	 * @param maxchar The maximum number of characters allowed in the input string.
	 *
	 * @returns char* A pointer to the user's input string.
	 */
	char *oxy_StringInput(const char title[], uint16_t x, uint8_t y, uint8_t maxchar);

	// Others.
	/**
	 * @brief Draws a color picker at the specified coordinates.
	 *
	 * This function renders the current palette on the screen at given X and Y position.
	 *
	 * @param cur_select Preset color (values range from 0-255).
	 * @param x X coordinate of the color picker.
	 * @param y Y coordinate of the color picker.
	 *
	 * @returns The selected color (values range from 0-255).
	 */
	uint8_t oxy_ColorPicker(uint8_t cur_select, uint16_t x, uint8_t y);

	/**
	 * @brief Prints the current time at the specified coordinates.
	 *
	 * This function prints the time at the given X and Y position.
	 *
	 * @param x X coordinate of the time display.
	 * @param y Y coordinate of the time display.
	 */
	void oxy_PrintTime(uint16_t x, uint8_t y);

	/**
	 * @brief Prints the current date at the specified coordinates.
	 *
	 * This function prints the current date on the screen at the specified coordinates (x, y).
	 * The date is displayed in the format "DD/MM/YYYY".
	 *
	 * @param x X coordinate of the date display.
	 * @param y Y coordinate of the date display.
	 */
	void oxy_PrintDate(uint16_t x, uint8_t y);

	/**
	 * @brief Renders the current battery status at the specified coordinates.
	 *
	 * This function is used to display an icon representing the current battery status at a specified x and y coordinate on the screen.
	 * The icon displayed will depend on the current battery level.
	 *
	 * @param x X coordinate
	 * @param y Y coordinate
	 */
	void oxy_PrintBatteryStatus(uint16_t x, uint8_t y);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __OXY_GUI_H__ */