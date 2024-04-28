/**
 * @file entry.h
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

#ifndef ENTRY_H
#define ENTRY_H

#include <tice.h>
#include <graphx.h>
#include "../globals.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

    /**
     * @brief Enum containing all the different types of input modes for an entry widget.
     *
     * @enum OXY_ENTRY_MODE_UPPER The entry widget will accept only uppercase letters as input.
     * @enum OXY_ENTRY_MODE_LOWER The entry widget will accept only lowercase letters as input.
     * @enum OXY_ENTRY_MODE_NUM The entry widget will accept only numeric input.
     */
    enum entry_mode_t
    {
        OXY_ENTRY_MODE_UPPER,
        OXY_ENTRY_MODE_LOWER,
        OXY_ENTRY_MODE_NUM,
    };

    /**
     * @brief Entry widget
     *
     * @param text The text currently displayed in the entry widget.
     * @param widget The base widget structure for this entry widget.
     * @param cursor_offset The position of the cursor within the text.
     * @param current_line The current line of the text being displayed.
     * @param mode The input mode for the entry widget.
     * @param max The maximum number of characters that can be entered into the widget.
     */
    struct oxy_entry_t
    {
        char *text;
        struct oxy_widget_t widget;
        enum entry_mode_t mode;
        
        uint8_t cursor_pos;
        uint8_t max_chars;
        bool password;

        uint8_t select_start;
        uint8_t select_end;
    };

    /**
     * @brief Update the state of an entry widget.
     *
     * This function updates the state of an entry widget, such as its text or cursor position, based on user input or other events.
     *
     * @param widget A pointer to the entry widget to be updated. The widget must be initialized before calling this function.
     *
     * @return void
     */
    void oxy_UpdateEntry(struct oxy_widget_t *widget);

    /**
     * @brief Render an entry widget on the screen.
     *
     * This function draws an entry widget on the screen, including its text and cursor.
     *
     * @param widget A pointer to the entry widget to be rendered. The widget must be initialized before calling this function.
     *
     * @return void
     */
    void oxy_RenderEntry(struct oxy_widget_t *widget);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ENTRY_H */