/**
 * @file menu.h
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

#ifndef MENU_H
#define MENU_H

#include <tice.h>
#include <graphx.h>
#include "../globals.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

    /**
     * @brief A menu widget
     *
     * The menu widget is represented by the oxy_menu_t struct, which contains fields for storing the widget's text,
     * sprites, selection, number of rows, and number of columns. The oxy_UpdateMenu and oxy_RenderMenu functions
     * can be used to update and render the menu widget, respectively.
     *
     * @struct oxy_menu_t
     * @brief Struct for storing menu widget data
     * @param widget The base widget structure for this menu widget
     * @param text An array of strings containing the text for each menu item
     * @param sprite An array of sprite pointers for each menu item
     * @param selection The index of the currently selected menu item
     * @param rows The number of rows in the menu
     * @param columns The number of columns in the menu
     */
    struct oxy_menu_t
    {
        struct oxy_widget_t widget;

        char **text;
        gfx_sprite_t **sprite;

        uint8_t  selection;
        uint8_t  rows;
        uint8_t  columns;
    };

    /**
     * @brief Update a menu widget based on user input or other events.
     *
     * This function updates the state of a menu widget, such as the selected item, based on user input or other events.
     *
     * @param widget A pointer to the menu widget to be updated. The widget must be initialized before calling this function.
     *
     * @return void
     */
    void oxy_UpdateMenu(struct oxy_widget_t *widget);

    /**
     * @brief Renders a menu widget on screen based on the input widget
     * 
     * This function takes in a pointer to a menu widget and renders it on the screen. The menu widget contains information such as the text and sprite to be displayed, the number of rows and columns, and the currently selected item.
     * 
     * @param widget A pointer to the menu widget to be rendered. The widget must be initialized before calling this function.
     *
     * @return void
     */
    void oxy_RenderMenu(struct oxy_widget_t *widget);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* OXY_MENU_H */