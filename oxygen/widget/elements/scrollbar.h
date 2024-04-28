/**
 * @file scrollbar.h
 * 
 * @author Alvajoy "Alvajoy123" Asante
 * @version 1.0
 * @copyright Copyright (c) 2018 - 2023
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

#ifndef SCROLLBAR_H
#define SCROLLBAR_H

#include <tice.h>
#include "../globals.h"

#define SCROLLBAR_HORIZONTAL 0
#define SCROLLBAR_VERTICAL 1

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

	/**
	 * @brief A scrollbar widget
	 *
	 * The scrollbar widget is represented by the oxy_scrollbar_t struct, which contains fields for storing the widget's alignment,
	 * and current value. The oxy_UpdateScrollbar and oxy_RenderScrollbar functions can be used to update and render the scrollbar widget, respectively.
	 *
	 * @struct oxy_scrollbar_t
	 * @brief Struct for storing scrollbar widget data
	 * @param widget The base widget structure for this scrollbar widget
	 * @param alignment A boolean value indicating the alignment of the scrollbar (vertical or horizontal)
	 * @param value The current value of the scrollbar
	 */
	struct oxy_scrollbar_t
	{
		struct oxy_widget_t widget;
		bool alignment;
		
		int min_value;
		int max_value;
		int current_value;
	};

	/**
	 * @brief Update a scrollbar widget based on user input or other events.
	 *
	 * This function updates the state of a scrollbar widget, such as the current value, based on user input or other events.
	 *
	 * @param widget A pointer to the scrollbar widget to be updated. The widget must be initialized before calling this function.
	 *
	 * @return void
	 */
	void oxy_UpdateScrollbar(struct oxy_widget_t *widget);

	/**
	 * @brief Renders a scrollbar widget on screen based on the input widget
	 *
	 * This function takes in a pointer to a scrollbar widget and renders it on the screen. The scrollbar widget contains information such as the alignment and current value of the widget.
	 *
	 * @param widget A pointer to the scrollbar widget to be rendered. The widget must be initialized before calling this function.
	 *
	 * @return void
	 */
	void oxy_RenderScrollbar(struct oxy_widget_t *widget);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* SCROLLBAR_H */