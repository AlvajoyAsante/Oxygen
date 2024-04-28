/**
 * @file checkbox.h
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

#ifndef CHECKBOX_H
#define CHECKBOX_H

#include <tice.h>
#include "../globals.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

	/**
	 * @brief A checkbox widget
	 *
	 * A checkbox is a small box that can be toggled on or off to represent a binary choice or selection.
	 *
	 * @param widget The base widget structure for this checkbox widget.
	 * @param on Whether the checkbox is currently checked or unchecked.
	 */
	struct oxy_checkbox_t
	{
		struct oxy_widget_t *widget;
		bool on;
	};

	/**
	 * @brief Update the state of a checkbox widget.
	 *
	 * This function updates the state of a checkbox widget based on user input or other events. For example, it might toggle the "on" state of the checkbox if it is being clicked.
	 *
	 * @param widget A pointer to the checkbox widget to be updated. The widget must be initialized before calling this function.
	 *
	 * @return void
	 */
	void oxy_UpdateCheckbox(struct oxy_widget_t *widget);

	/**
	 * @brief Render a checkbox widget on the screen.
	 *
	 * This function draws a checkbox widget on the screen, including its label and any visual states (e.g. checked, unchecked).
	 *
	 * @param widget A pointer to the checkbox widget to be rendered. The widget must be initialized before calling this function.
	 *
	 * @return void
	 */
	void oxy_RenderCheckbox(struct oxy_widget_t *widget);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* OXY_CHECKBOX_H */