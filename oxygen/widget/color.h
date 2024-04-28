/**
 * @file color.h
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

#ifndef COLOR_H
#define COLOR_H

#include <tice.h>
#include "globals.h"

/* Text color defaults */
#define OXY_DEFAULT_TEXT_FG_COLOR 255
#define OXY_DEFAULT_TEXT_BG_COLOR 0

#define OXY_DEFAULT_UNSELECTED_COLOR 74
#define OXY_DEFAULT_SELECTED_COLOR 74
#define OXY_DEFAULT_CLICKED_COLOR 74

/* Extra color defaults */
#define OXY_DEFAULT_COLOR_A_COLOR 0
#define OXY_DEFAULT_COLOR_B_COLOR 0
#define OXY_DEFAULT_COLOR_C_COLOR 0

/* Window colors defaults */
#define OXY_WINDOW_FILL_COLOR 255
#define OXY_WINDOW_BORDER_COLOR_A 0
#define OXY_WINDOW_BORDER_COLOR_B 1

#define OXY_WINDOW_TITLE_TEXT_FG_UNSELECTED_COLOR 0
#define OXY_WINDOW_TITLE_TEXT_BG_UNSELECTED_COLOR 255
#define OXY_WINDOW_UNSELECTED_COLOR 255

#define OXY_WINDOW_TITLE_TEXT_FG_SELECTED_COLOR 255
#define OXY_WINDOW_TITLE_TEXT_BG_SELECTED_COLOR 0
#define OXY_WINDOW_SELECTED_COLOR 18

/* Button Color defaults */
#define OXY_BUTTON_FILL_COLOR 0
#define OXY_BUTTON_BORDER_COLOR 8

#define OXY_BUTTON_TEXT_FG_UNSELECTED_COLOR 255
#define OXY_BUTTON_TEXT_BG_UNSELECTED_COLOR 0
#define OXY_BUTTON_BG_UNSELECTED_COLOR 0

#define OXY_BUTTON_TEXT_FG_SELECTED_COLOR 0
#define OXY_BUTTON_TEXT_BG_SELECTED_COLOR 255
#define OXY_BUTTON_BG_SELECTED_COLOR 255

#define OXY_BUTTON_TEXT_FG_CLICKED_COLOR 255
#define OXY_BUTTON_TEXT_BG_CLICKED_COLOR 0
#define OXY_BUTTON_BG_CLICKED_COLOR 18

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

    /**
     * @brief Sets Extra widget colors
     *
     * @param widget A pointer to the widget whose extra colors are being set
     * @param color_a The color to set for the first extra color
     * @param color_b The color to set for the second extra color
     * @param color_c The color to set for the third extra color
     *
     * @return void
     */
    void oxy_SetWidgetExtraColors(struct oxy_widget_t *widget, uint8_t color_a, uint8_t color_b, uint8_t color_c);

    /**
     * @brief Sets widget unselected color
     *
     * @param widget A pointer to the widget whose unselected color is being set
     * @param color The color to set for the unselected state
     * @param text_fg The text foreground color to set for the unselected state
     * @param text_bg The text background color to set for the unselected state
     *
     * @return void
     */
    void oxy_SetWidgetUnselectedColor(struct oxy_widget_t *widget, uint8_t color, uint8_t text_fg, uint8_t text_bg);

    /**
     * @brief Sets widget selected color
     *
     * @param widget A pointer to the widget whose selected color is being set
     * @param color The color to set for the selected state
     * @param text_fg The text foreground color to set for the selected state
     * @param text_bg The text background color to set for the selected state
     *
     * @return void
     */
    void oxy_SetWidgetSelectedColor(struct oxy_widget_t *widget, uint8_t color, uint8_t text_fg, uint8_t text_bg);

    /**
     * @brief Sets widget clicked color
     *
     * This function sets the color and text color of a widget when it is clicked.
     *
     * @param widget A pointer to the widget whose clicked color is to be set. The widget must be initialized before calling this function.
     * @param color The color to set the widget to when it is clicked.
     * @param text_fg The foreground text color to set for the widget when it is clicked.
     * @param text_bg The background text color to set for the widget when it is clicked.
     *
     * @return void
     */
    void oxy_SetWidgetClickedColor(struct oxy_widget_t *widget, uint8_t color, uint8_t text_fg, uint8_t text_bg);

    /**
     * @brief Sets widgets to default colors
     *
     * This function sets the colors of a widget to the default values for its type.
     *
     * @param widget A pointer to the widget whose colors are to be set to the default values. The widget must be initialized before calling this function.
     * @param type The type of the widget, which determines the default colors to set.
     *
     * @return void
     */
    void oxy_SetDefaultColors(struct oxy_widget_t *widget, uint8_t type);

    /**
     * @brief Returns the color struct for a widget
     * 
     * This function returns the color struct for a widget, which contains information about the colors to be used for the different states of the widget (unselected, selected, clicked, etc.).
     *
     * @param widget A pointer to the widget for which to retrieve the color struct. The widget must be initialized before calling this function.
     * 
     * @returns A struct containing the colors for the different states of the widget.
     */
    struct oxy_color_t oxy_GetWidgetColor(struct oxy_widget_t *widget);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* COLOR_H */