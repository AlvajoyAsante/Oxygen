/**
 * @file util.h
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

#ifndef OXY_UTIL_H
#define OXY_UTIL_H

#include <tice.h>
#include <graphx.h>
#include <math.h>
#include <string.h>

#include "globals.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

    // initializations
    /**
     * @brief Initiation of Widget struct
     * 
     * @param widget widget struct you want to initialize
     * @param type type of widget you want to initialize
     *
     */
    bool oxy_InitWidget(struct oxy_widget_t *widget, uint8_t type);

    // Cursor
    /**
     * @brief Set widget cursor description
     * 
     * @param widget widget struct you want to set the cursor description
     * @param text text you want to set the cursor description
     *
     */
    void oxy_SetWidgetDescription(struct oxy_widget_t *widget, char *text);

    /**
     * @brief Set Widget cursor right click callback
     * 
     * @param widget widget struct you want to set the cursor right click callback
     * @param function function you want to set the cursor right click callback
     * @param arg argument you want to set the cursor right click callback
     *
     */
    void oxy_SetWidgetRightClick(struct oxy_widget_t *widget, void *function, void *arg);

    /**
     * @brief Set Widget cursor Left Click callback
     * 
     * @param widget widget struct you want to set the cursor left click callback
     * @param function function you want to set the cursor left click callback
     * @param arg argument you want to set the cursor left click callback
     *
     */
    void oxy_SetWidgetLeftClick(struct oxy_widget_t *widget, void *function, void *arg);

    /**
     * @brief Set Widget cursor hover callback
     * 
     * @param widget widget struct you want to set the cursor hover callback
     * @param function function you want to set the cursor hover callback
     * @param arg argument you want to set the cursor hover callback
     *
     */
    void oxy_SetWidgetHover(struct oxy_widget_t *widget, void *function, void *arg);

    // Widget Transformation
    /**
     * @brief Set label X alignment
     * @param widget widget struct make sure you init the widget before loading.
     * @param alignment alignment you want the label to display.
     */
    void oxy_SetWidgetXalign(struct oxy_widget_t *widget, uint8_t alignment);

    /**
     * @brief Set label Y alignment
     * @param widget widget struct make sure you init the widget before loading.
     * @param alignment alignment you want the label to display.
     */
    void oxy_SetWidgetYalign(struct oxy_widget_t *widget, uint8_t alignment);

    /**
     * @brief Sets Widget width and height
     * 
     * @param widget widget struct you want to set the width and height
     * @param width width you want to set the widget
     * @param height height you want to set the widget
     *
     */
    void oxy_SetWidgetSize(struct oxy_widget_t *widget, uint16_t width, uint8_t height);

    /**
     * @brief Sets Widget x and y position
     * 
     * @param widget widget struct you want to set the x and y position
     * @param x x position you want to set the widget
     * @param y y position you want to set the widget
     *
     */
    void oxy_SetWidgetPosition(struct oxy_widget_t *widget, int x, int y);

    /**
     * @brief Sets Widget Render and Update callback
     *
     * @param widget widget struct you want to set the render and update callback
     * @param render render callback you want to set the widget
     * @param update update callback you want to set the widget
     * 
     */
    void oxy_SetWidgetCallback(struct oxy_widget_t *widget, void (*render)(struct oxy_widget_t *), void (*update)(struct oxy_widget_t *));

    /**
     * @brief Sets Widget to a list of widgets (best for menus and list)
     * 
     * @param widget widget struct you want to set the widget children
     * @param children list of widgets you want to set the widget children
     *
     */
    void oxy_SetWidgetChildren(struct oxy_widget_t *widget, struct oxy_widget_t **children);

    // Aligning and rendering
    /**
     * @brief Aligns a source widget to a destination widget according to the specified horizontal and vertical alignments.
     * @note This function does not modify the source or destination widgets.
     *
     * @param src_widget Pointer to the source widget to be aligned.
     * @param dest_widget Pointer to the destination widget to which the source widget will be aligned.
     * @param x_align Horizontal alignment of the source widget relative to the destination widget.
     * @param y_align Vertical alignment of the source widget relative to the destination widget.
     *
     * @return void
     */
    void oxy_AlignWidgetToWidget(struct oxy_widget_t *src_widget, struct oxy_widget_t *dest_widget, uint8_t x_align, uint8_t y_align);

    /**
     * @brief Aligns a widget to the screen according to the specified horizontal and vertical alignments.
     *
     * @param widget Pointer to the widget to be aligned.
     */
    void oxy_AlignChildren(struct oxy_widget_t *widget);

    // Stack Management
    /**
     * @brief Forces stack widgets to update and render
     *
     *  @param stack a array of widgets
     */
    void oxy_StackForceRedraw(struct oxy_widget_t **stack);

    /**
     * @brief Checks if the widgets need to be redrawn
     * 
     * @param stack a array of widgets
     * @param reference a reference widget to check if the stack needs to be redrawn
     */
    void oxy_StackCheckRedraw(struct oxy_widget_t **stack, struct oxy_widget_t *reference);

    /**
     * @brief Gets index that widget is positioned in!
     * @param stack a array of widgets
     * @param widget a widget you want to check
     * @returns stack index that needs to be redrawn return a -1 if none
     */
    int oxy_GetWidgetStackIndex(struct oxy_widget_t **stack, struct oxy_widget_t *widget);

    /**
     * @brief Return the cursors length
     * 
     * @param stack a array of widgets
     * 
     * @returns stack index that needs to be redrawn return a -1 if none
     */
    uint16_t oxy_GetStackLength(struct oxy_widget_t **stack);

    // Cursor Detecting
    /**
     * @brief Check if the cursor is overlapping "dest_widget"
     * 
     * @param dest_widget widget you want to check if the cursor is overlapping
     * 
     * @returns bool if cursor is overlapping with "dest_widget" returns true else false
     */
    bool oxy_CheckCursorOverlap(struct oxy_widget_t *dest);

    /**
     * @brief Checks if two widgets overlap
     * 
     * @param src starting widget you want to compare
     * @param dest ending widget you want to compare
     * 
     * @returns bool if widgets overlap then returns true else false
     */
    bool oxy_CheckWidgetOverlap(struct oxy_widget_t *src, struct oxy_widget_t *dest);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* OXY_UTIL_H */