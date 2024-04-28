/**
 * @file sprite.h
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

#ifndef SPRITE_H
#define SPRITE_H

#include <tice.h>
#include <graphx.h>
#include "../globals.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

	/**
	 * @brief A sprite widget
	 *
	 * The sprite widget is represented by the oxy_sprite_t struct, which contains fields for storing the widget's sprite,
	 * scaling factors, and transparency information. The oxy_RenderSprite function can be used to render the sprite widget
	 * on the screen.
	 *
	 * @struct oxy_sprite_t
	 * @brief Struct for storing sprite widget data
	 * @param spr A pointer to the sprite to be displayed
	 * @param widget The base widget structure for this sprite widget
	 * @param x_scale The scaling factor for the sprite's x-dimension
	 * @param y_scale The scaling factor for the sprite's y-dimension
	 * @param transparent A flag indicating whether the sprite should be rendered with transparency
	 * @param transparent_color_index The index of the transparent color in the sprite's palette, if applicable
	 */
	struct oxy_sprite_t
	{
		struct oxy_widget_t widget;

		gfx_sprite_t *spr;
		
		uint8_t x_scale;
		uint8_t y_scale;

		bool transparent;
		uint8_t transparent_color_index;
	};

	/**
	 * @brief Renders a sprite widget on screen based on the input widget
	 *
	 * This function takes in a pointer to a sprite widget and renders it on the screen. The sprite widget contains information
	 * such as the sprite to be displayed, scaling factors, and transparency settings.
	 *
	 * @param widget A pointer to the sprite widget to be rendered. The widget must be initialized before calling this function.
	 *
	 * @return void
	 */
	void oxy_RenderSprite(struct oxy_widget_t *widget);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* SPRITE_H */