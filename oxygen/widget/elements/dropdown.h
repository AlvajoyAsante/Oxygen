/**
 * @file dropdown.h
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

#ifndef DROPDOWN_H
#define DROPDOWN_H

#include <tice.h>
#include "../globals.h"
#include "menu.h"
#include "button.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

    /**
     * @brief A structure representing a dropdown widget.
     *
     * The dropdown widget consists of a button that can be clicked to open a menu of options. The user can then select an option from the menu.
     *
     * @param widget The base widget structure for this dropdown widget.
     * @param menu The menu structure containing the options for the dropdown.
     * @param button The button widget for opening the menu.
     */
    struct oxy_dropdown_t
    {
        struct oxy_widget_t widget;
        struct oxy_menu_t *menu;
        struct oxy_widget_t *button;
    };

    /**
     * @brief Update the state of a dropdown widget.
     *
     * This function updates the state of a dropdown widget based on user input or other events. It may update the selected menu item, for example, if the user clicks on a different item in the dropdown menu.
     *
     * @param widget A pointer to the dropdown widget to be updated. The widget must be initialized before calling this function.
     *
     * @return void
     */
    void oxy_UpdateDropDown(struct oxy_widget_t *widget);

    /**
     * @brief Render a dropdown widget on the screen.
     *
     * This function draws a dropdown widget on the screen, including its button and menu.
     *
     * @param widget A pointer to the dropdown widget to be rendered. The widget must be initialized before calling this function.
     *
     * @return void
     */
    void oxy_RenderDropDown(struct oxy_widget_t *widget);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* OXY_DROPDOWN_H */