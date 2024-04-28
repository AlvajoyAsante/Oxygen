/**
 * @file window.h
 *
 * @author Alvajoy "Alvajoy123" Asante
 * @version 1.0
 * @copyright Copyright (c) 2018 - 2022
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

#ifndef WINDOW_H
#define WINDOW_H

#include <tice.h>
#include "../globals.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

	/**
	 * @brief A window widget, struct for storing window widget data
	 *
	 * The window widget is represented by the oxy_window_t struct, which contains fields for storing the widget's title,
	 * width, and height. The oxy_UpdateWindow, oxy_RenderWindow, oxy_ResizeWindow, oxy_IsWindowVisable, and oxy_IsWindowViewable functions
	 * can be used to update, render, resize, check the visibility, and check the viewability of the window widget, respectively.
	 *
	 * @param widget The base widget structure for this window widget
	 * @param child A pointer to an array of child widgets belonging to this window widget
	 * @param title The string containing the title for the window
	 * @param closeable A boolean value indicating whether the window can be closed or not
	 * @param minimizable A boolean value indicating whether the window can be minimized or not
	 * @param on_close A function pointer to a function that is called when the window is closed
	 * @param minimize_button A pointer to a button widget used to minimize the window
	 * @param close_button A pointer to a button widget used to close the window
	 * @param header_bar A pointer to a rectangle widget used to represent the header bar of the window
	 * @param maximize_button A pointer to a button widget used to maximize the window
	 */
	struct oxy_window_t
	{
		struct oxy_widget_t widget;
		// struct oxy_widget_t *child;

		char *title;

		bool closeable;
		bool minimizable;

		void (*on_close)(void *);

		struct oxy_button_t *close_button;
		struct oxy_button_t *minimize_button;
		struct oxy_button_t *maximize_button;
		struct oxy_rectangle_t *header_bar;
	};

	/**
	 * @brief Update a window widget based on user input or other events.
	 *
	 * This function updates the state of a window widget based on user input or other events. It may update the size or position of the window, for example.
	 *
	 * @param widget A pointer to the window widget to be updated. The widget must be initialized before calling this function.
	 *
	 * @return void
	 */
	void oxy_UpdateWindow(struct oxy_widget_t *widget);

	/**
	 * @brief Render a window widget on screen based on the input widget
	 *
	 * This function takes in a pointer to a window widget and renders it on the screen. The window widget contains information such as the title, width, and height.
	 *
	 * @param widget A pointer to the window widget to be rendered. The widget must be initialized before calling this function.
	 *
	 * @return void
	 */
	void oxy_RenderWindow(struct oxy_widget_t *widget);

	/**
	 * @brief Resize a window widget
	 *
	 * This function resizes a window widget based on the specified width and height.
	 *
	 * @param widget A pointer to the window widget to be resized. The widget must be initialized before calling this function.
	 * @param width The new width of the window
	 * @param height The new height of the window
	 *
	 * @return void
	 */
	void oxy_ResizeWindow(struct oxy_widget_t *widget, uint16_t width, uint8_t height);

	/**
	 * @brief Check if a window is currently visible.
	 *
	 * This function checks if a window is currently visible on the screen. A window is considered visible if it is within the boundaries of the screen and not obscured by another window.
	 *
	 * @param window A pointer to the window to check.
	 *
	 * @return true if the window is visible, false otherwise.
	 */
	bool oxy_IsWindowVisable(struct oxy_window_t *window);

	/**
	 * @brief Check if a window is viewable from another window.
	 *
	 * This function checks if a window is visible from another window, taking into account the positioning of the windows and any obscuring windows.
	 *
	 * @param window A pointer to the window to check.
	 * @param reference A pointer to the reference window.
	 *
	 * @return true if the window is viewable from the reference window, false otherwise.
	 */
	bool oxy_IsWindowViewable(struct oxy_window_t *window, struct oxy_window_t *reference);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* WINDOW_H */