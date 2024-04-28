/**
 * @file colorbox.h
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

#ifndef COLORBOX_H
#define COLORBOX_H

#include <tice.h>
#include "../globals.h"
#include "button.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

	/**
	 * @brief A colorbox widget.
	 *
	 * A colorbox widget is a graphical element that allows the user to select a color. It consists of a small square that displays the currently selected color, and a button that can be clicked to open a color picker dialog.
	 *
	 * @param widget A pointer to the base widget structure for this colorbox widget.
	 * @param index The index of the selected color in the palette.
	 * @param color The value of the selected color.
	 */
	struct oxy_colorbox_t
	{
		struct oxy_widget_t *widget;
		uint8_t index;
	};

	/**
	 * @brief Update the state of a colorbox widget.
	 *
	 * This function updates the state of a colorbox widget based on user input or other events. It may update the selected color, for example, if the user clicks the button to open the color picker dialog.
	 *
	 * @param widget A pointer to the colorbox widget to be updated. The widget must be initialized before calling this function.
	 *
	 * @return void
	 */
	void oxy_UpdateColorbox(struct oxy_widget_t *widget);

	/**
	 * @brief Render a colorbox widget on the screen.
	 *
	 * This function draws a colorbox widget on the screen, including the selected color and the button to open the color picker dialog.
	 *
	 * @param widget A pointer to the colorbox widget to be rendered. The widget must be initialized before calling this function.
	 *
	 * @return void
	 */
	void oxy_RenderColorbox(struct oxy_widget_t *widget);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* OXY_UTIL_H */