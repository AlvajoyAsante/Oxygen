/**
 * @file button.h
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

#ifndef BUTTON_H
#define BUTTON_H

#include <tice.h>
#include "../globals.h"

#include "label.h"
#include "sprite.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

    /**
     * @brief A button widget
     *
     * A button is a clickable area on the screen that can be used to trigger an action. A button usually contains text or other information that describes the action it will perform when clicked.
     *
     * @param widget The base widget structure for this button widget.
     * @param label The text label displayed on the button.
     * @param sprite The sprite displayed on the button.
     */
    struct oxy_button_t
    {
        struct oxy_widget_t widget;
        struct oxy_label_t *label;
        struct oxy_sprite_t *sprite;
    };

    /**
     * @brief Update the state of a button widget.
     *
     * This function updates the state of a button widget based on user input or other events. For example, it might change the appearance of the button if it is being hovered over or clicked.
     *
     * @param widget A pointer to the button widget to be updated. The widget must be initialized before calling this function.
     *
     * @return void
     */
    void oxy_UpdateButton(struct oxy_widget_t *widget);

    /**
     * @brief Render a button widget on the screen.
     *
     * This function draws a button widget on the screen, including its label and any visual states (e.g. hover, active).
     *
     * @param widget A pointer to the button widget to be rendered. The widget must be initialized before calling this function.
     *
     * @return void
     */
    void oxy_RenderButton(struct oxy_widget_t *widget);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* BUTTON_H */