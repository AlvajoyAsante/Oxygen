/**
 * @file rectangle.h
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

#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <tice.h>
#include "../globals.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

    /**
     * @brief A rectangle widget.
     *
     * The rectangle widget is represented by the oxy_rectangle_t struct, which contains fields for storing the widget's
     * roundness, fill, and border properties. The oxy_UpdateRectangle and oxy_RenderRectangle functions can be used to
     * update and render the rectangle widget, respectively.
     *
     * @struct oxy_rectangle_t
     * @brief Struct for storing rectangle widget data
     * @param widget The base widget structure for this rectangle widget
     * @param round Whether the rectangle has rounded corners
     * @param filled Whether the rectangle is filled or not
     * @param border Whether the rectangle has a border or not
     */
    struct oxy_rectangle_t
    {
        struct oxy_widget_t widget;
        bool round;
        bool filled;
        bool border;
    };

    /**
     * @brief Update a rectangle widget based on user input or other events.
     *
     * This function updates the state of a rectangle widget, such as its fill and border properties, based on user input
     * or other events.
     *
     * @param widget A pointer to the rectangle widget to be updated. The widget must be initialized before calling this function.
     *
     * @return void
     */
    void oxy_UpdateRectangle(struct oxy_widget_t *widget);

    /**
     * @brief Render a rectangle widget on screen based on the input widget
     *
     * This function takes in a pointer to a rectangle widget and renders it on the screen. The rectangle widget contains
     * information such as its roundness, fill and border properties.
     *
     * @param widget A pointer to the rectangle widget to be rendered. The widget must be initialized before calling this function.
     *
     * @return void
     */
    void oxy_RenderRectangle(struct oxy_widget_t *widget);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* RECTANGLE_H */