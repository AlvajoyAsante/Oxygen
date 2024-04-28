/**
 * @file vmargin.h
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

#ifndef VMARGIN_H
#define VMARGIN_H

#include <tice.h>
#include "../globals.h"

#define VMARGIN_ALIGN_LEFT 0
#define VMARGIN_ALIGN_RIGHT 1

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

	/**
	 * @brief A vertical margin widget
	 *
	 * The vertical margin widget is represented by the oxy_vmargin_t struct, which contains a field for storing the widget's alignment. The oxy_RenderVmargin function can be used to render the vertical margin widget.
	 *
	 * @struct oxy_vmargin_t
	 * @brief Struct for storing vertical margin widget data
	 * @param widget The base widget structure for this vertical margin widget
	 * @param alignment The alignment of the vertical margin (left, center, or right)
	 */
	struct oxy_vmargin_t
	{
		struct oxy_widget_t widget;
		enum oxy_align_t alignment;
	};

	/**
	 * @brief Renders a vertical margin widget on the screen
	 *
	 * This function takes in a pointer to a vertical margin widget and renders it on the screen. The vertical margin widget contains information such as its alignment.
	 *
	 * @param widget A pointer to the vertical margin widget to be rendered. The widget must be initialized before calling this function.
	 *
	 * @return void
	 */
	void oxy_RenderVmargin(struct oxy_widget_t *widget);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* VMARGIN_H */