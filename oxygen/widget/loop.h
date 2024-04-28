/**
 * @file loop.h
 *
 * @author Alvajoy "Alvajoy123" Asante
 * @version 1.0
 * @copyright Copyright (c) 2018 - 2024
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

#ifndef LOOP_H
#define LOOP_H

#include <tice.h>
#include "globals.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

    /**
     * @brief Updates widget in given stack
     *
     * @note This function updates the widgets in the given stack by calling their update functions. This allows the widgets to perform any necessary tasks, such as redrawing themselves or responding to user input.
     *
     * @param stack A pointer to the stack of widgets to update
     *
     */
    void oxy_UpdateStack(struct oxy_widget_t **stack);

    /**
     * @brief Updates all visible widgets
     *
     * @note This function iterates through all the widgets in the widget stack and updates the ones that are visible.
     * This may include processing user input, changing widget state, or updating widget data.
     *
     */
    void oxy_UpdateWidgets(void);

    /**
     * @brief Renders widget in given stack
     *
     * @note function renders the widgets in the given stack by calling their render functions.
     * This draws the widgets onto the screen, allowing the user to see and interact with them.
     *
     * @param stack A pointer to the stack of widgets to render
     *
     */
    void oxy_RenderStack(struct oxy_widget_t **stack);

    /**
     * @brief Renders all visible widgets
     *
     * @note This function iterates through all the widgets in the widget stack and renders the ones that are visible.
     * This may include drawing the widget on screen or updating the screen with the widget's new state.
     *
     * @return void
     */
    void oxy_RenderWidgets(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LOOP_H */