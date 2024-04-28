/**
 * @file label.h
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

#ifndef LABEL_H
#define LABEL_H

#include <tice.h>
#include <graphx.h>
#include <string.h>
#include "../globals.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

    /**
     * @brief Struct for a label widget.
     *
     * A label widget is used to display text on the screen. It has a variety of options for formatting the text, such as wrapping and alignment.
     *
     * @param widget The base widget structure for this label widget.
     * @param text The text to be displayed in the label.
     * @param wrap Whether the text should wrap to the next line if it exceeds the width of the widget.
     * @param start_line The first line of text to be displayed.
     * @param max_lines The maximum number of lines to display.
     */
    struct oxy_label_t
    {
        struct oxy_widget_t widget;
        
        char *text;
        
        bool wrap;
        int start_line;
        int max_lines;
    };

    /**
     * @brief Renders a label widget on the screen.
     *
     * This function draws a label widget on the screen, including its text and formatting.
     *
     * @param widget A pointer to the label widget to be rendered. The widget must be initialized before calling this function.
     *
     * @return void
     */
    void oxy_RenderLabel(struct oxy_widget_t *widget);

    /**
     * @brief Sets the text of a label widget.
     *
     * This function updates the text displayed in a label widget.
     *
     * @param widget A pointer to the label widget whose text is to be set. The widget must be initialized before calling this function.
     * @param text The text to be displayed in the label.
     *
     * @return void
     */
    void oxy_SetLabelText(struct oxy_widget_t *widget, const char *text);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* TEXT_H */