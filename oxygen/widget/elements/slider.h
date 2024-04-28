/**
 * @file slider.h
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

#ifndef SLIDER_H
#define SLIDER_H

#include <tice.h>
#include "../globals.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

	/**
	 * @brief A widget that allows a user to select a value within a range using a sliding handle.
	 *
	 * The slider widget is represented by the oxy_slider_t struct, which contains fields for storing the widget's alignment,
	 * rendering value flag, minimum and maximum values, and current value. The oxy_UpdateSlider and oxy_RenderSlider functions
	 * can be used to update and render the slider widget, respectively.
	 *
	 * @struct oxy_slider_t
	 * @brief Struct for storing slider widget data
	 * @param widget The base widget structure for this slider widget
	 * @param alignment The alignment of the slider widget, either horizontal or vertical
	 * @param render_value A flag indicating whether the current value should be displayed on the widget
	 * @param min The minimum value that can be selected on the slider
	 * @param max The maximum value that can be selected on the slider
	 * @param curr_value The current value of the slider
	 */
	struct oxy_slider_t
	{
		struct oxy_widget_t widget;
		bool alignment;
		bool render_value;

		int min;
		int max;
		int step;
		int curr_value;
	};

	/**
	 * @brief Update a slider widget based on user input or other events.
	 *
	 * This function updates the state of a slider widget, such as the current value, based on user input or other events.
	 *
	 * @param widget A pointer to the slider widget to be updated. The widget must be initialized before calling this function.
	 *
	 * @return void
	 */
	void oxy_UpdateSlider(struct oxy_widget_t *widget);

	/**
	 * @brief Renders a slider widget on screen based on the input widget
	 *
	 * This function takes in a pointer to a slider widget and renders it on the screen. The slider widget contains information such as the alignment, minimum and maximum values, and current value of the slider.
	 *
	 * @param widget A pointer to the slider widget to be rendered. The widget must be initialized before calling this function.
	 *
	 * @return void
	 */
	void oxy_RenderSlider(struct oxy_widget_t *widget);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* SLIDER_H */