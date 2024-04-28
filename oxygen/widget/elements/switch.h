/**
 * @file switch.h
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

#ifndef SWITCH_H
#define SWITCH_H

#include <tice.h>
#include "../globals.h"

#define OXY_DEFAULT_SWITCH_WIDTH 10
#define OXY_DEFAULT_SWITCH_HEIGHT 8

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

	/**
	 * @brief A switch widget
	 *
	 * The switch widget is represented by the oxy_switch_t struct, which contains a field for storing the widget's state (on or off). The oxy_UpdateSwitch and oxy_RenderSwitch functions can be used to update and render the switch widget, respectively.
	 *
	 * @struct oxy_switch_t
	 * @brief Struct for storing switch widget data
	 * @param widget The base widget structure for this switch widget
	 * @param value A boolean value indicating the current state of the switch (true for on, false for off)
	 */
	struct oxy_switch_t
	{
		struct oxy_widget_t widget;
		bool value;
	};

	/**
	 * @brief Update the state of a switch widget.
	 *
	 * This function updates the state of a switch widget based on user input or other events.
	 *
	 * @param widget A pointer to the switch widget to be updated. The widget must be initialized before calling this function.
	 *
	 * @return void
	 */
	void oxy_UpdateSwitch(struct oxy_widget_t *widget);

	/**
	 * @brief Renders a switch widget on screen based on the input widget
	 *
	 * This function takes in a pointer to a switch widget and renders it on the screen. The switch widget contains information such as its current state (on or off).
	 *
	 * @param widget A pointer to the switch widget to be rendered. The widget must be initialized before calling this function.
	 *
	 * @return void
	 */
	void oxy_RenderSwitch(struct oxy_widget_t *widget);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* SWITCH_H */