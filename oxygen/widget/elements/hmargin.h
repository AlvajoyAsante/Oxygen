/**
 * @file hmargin.h
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

#ifndef HMARGIN_H
#define HMARGIN_H

#include <tice.h>
#include "../globals.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

	/**
	 * @brief Struct representing a horizontal margin widget.
	 *
	 * @param widget The base widget structure for this horizontal margin widget.
	 * @param alignment The alignment of the margin. Can be set to either OXY_ALIGN_LEFT, OXY_ALIGN_CENTER, or OXY_ALIGN_RIGHT.
	 */
	struct oxy_hmargin_t
	{
		struct oxy_widget_t widget;
		enum oxy_align_t alignment;
	};

	/**
	 * @brief Renders a horizontal margin widget on the screen.
	 *
	 * This function draws a horizontal margin on the screen, according to the specified alignment.
	 *
	 * @param widget A pointer to the horizontal margin widget to be rendered. The widget must be initialized before calling this function.
	 *
	 * @return void
	 */
	void oxy_RenderHmargin(struct oxy_widget_t *widget);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* OXY_HMARGIN_H */