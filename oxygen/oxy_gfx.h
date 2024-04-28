/**
 * @file oxy_gfx.h
 * @brief This file contains all graphic functions, including functions for drawing shapes, setting pixels, and more.
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

#ifndef OXY_GFX_H
#define OXY_GFX_H

#include <tice.h>
#include <graphx.h>
#include <fileioc.h>
#include "oxy_gfx.h"

#define oxy_FillIndentedRectangle_All(x, y, w, h) \
    oxy_FillIndentedRectangle(x, y, w, h, 0)

#define oxy_FillIndentedRectangle_Upper(x, y, w, h) \
    oxy_FillIndentedRectangle(x, y, w, h, 1)

#define oxy_FillIndentedRectangle_Bottom(x, y, w, h) \
    oxy_FillIndentedRectangle(x, y, w, h, 2)

#define oxy_FillIndentedRectangle_Left(x, y, w, h) \
    oxy_FillIndentedRectangle(x, y, w, h, 3)

#define oxy_FillIndentedRectangle_Right(x, y, w, h) \
    oxy_FillIndentedRectangle(x, y, w, h, 4)

#define oxy_IndentedRectangle_All(x, y, w, h) \
    oxy_IndentedRectangle(x, y, w, h, 0)

#define oxy_IndentedRectangle_Upper(x, y, w, h) \
    oxy_IndentedRectangle(x, y, w, h, 1)

#define oxy_IndentedRectangle_Bottom(x, y, w, h) \
    oxy_IndentedRectangle(x, y, w, h, 2)

#define oxy_IndentedRectangle_Left(x, y, w, h) \
    oxy_IndentedRectangle(x, y, w, h, 3)

#define oxy_IndentedRectangle_Right(x, y, w, h) \
    oxy_IndentedRectangle(x, y, w, h, 4)

#define oxy_Rgb1555toGFX(x) \
    oxy_Rgb1555toPalette(x, gfx_palette, 256)

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

    /**
     * @brief Renders a checkered rectangle on the screen
     *
     * This function takes the x and y position, width, and height of a rectangle as input and uses a nested loop to render
     * a checkered pattern on the screen using gfx_SetPixel.
     *
     * @param x The x position of the rectangle
     * @param y The y position of the rectangle
     * @param w The width of the rectangle
     * @param h The height of the rectangle
     *
     * @return void
     */
    void oxy_CheckeredRectangle(uint16_t x, uint8_t y, int w, uint8_t h);

    /**
     * @brief Fills a rounded rectangle with a solid color
     *
     * This function fills a rounded rectangle with a solid color. The rounded rectangle is defined by its top-left corner
     * coordinates (x, y), width (w), height (h), and corner radius (r). The color of the rectangle is set using
     * gfx_SetColor().
     *
     * @param x The x-coordinate of the top-left corner of the rectangle
     * @param y The y-coordinate of the top-left corner of the rectangle
     * @param w The width of the rectangle
     * @param h The height of the rectangle
     * @param r The radius of the corners of the rectangle
     *
     * @return void
     */
    void oxy_FillRoundRectangle(uint16_t x, uint8_t y, int w, uint8_t h, uint8_t r);

    /**
     * @brief A rounded rectangle with a solid color
     *
     * This function a rounded rectangle with a solid color. The rounded rectangle is defined by its top-left corner
     * coordinates (x, y), width (w), height (h), and corner radius (r). The color of the rectangle is set using
     * gfx_SetColor().
     *
     * @param x The x-coordinate of the top-left corner of the rectangle
     * @param y The y-coordinate of the top-left corner of the rectangle
     * @param w The width of the rectangle
     * @param h The height of the rectangle
     * @param r The radius of the corners of the rectangle
     *
     * @return void
     */
    void oxy_RoundRectangle(int16_t x, int8_t y, int w, uint8_t h, uint8_t r);

    /**
     * @brief Renders a rectangle with a solid fill color and an outline of a different color
     *
     * This function takes in the top-left x-coordinate, top-left y-coordinate, width, height, fill color, and outline color of a rectangle and
     * renders the rectangle using the graphics library. The fill color is used to fill the inside of the rectangle, while the outline color is used
     * to draw the border of the rectangle.
     *
     * @param x The top-left x-coordinate of the rectangle
     * @param y The top-left y-coordinate of the rectangle
     * @param w The width of the rectangle
     * @param h The height of the rectangle
     * @param fill_color The color to use to fill the inside of the rectangle
     * @param outline_color The color to use to draw the border of the rectangle
     *
     * @return void
     */
    void oxy_OutlinedRectangle(uint16_t x, uint8_t y, int w, uint8_t h, uint8_t fill_color, uint8_t outline_color);

    /**
     * @brief Draws a outlined using oxygen round rectangle
     *
     * @param x X Positions
     * @param y Y Positions
     * @param w Width of Rectangle
     * @param h Height of Rectangle
     * @param fill_color Fill Color of inner rectangle
     * @param outline_color Outline Color of outer rectangle
     *
     * @return void
     */
    void oxy_OutlinedRoundRectangle(uint16_t x, uint8_t y, int w, uint8_t h, uint8_t type, uint8_t fill_color, uint8_t outline_color);

    /**
     * @brief Renders a filled rounded rectangle with an oxygen style.
     *
     * This function renders a filled rounded rectangle with a style similar to the one used in the Oxygen GUI library. The rectangle is drawn starting at the specified
     * position, with the specified width and height. The "type" parameter determines the style of the rounded corners, with values ranging from 0 to 4.
     *
     * @param x The x-coordinate of the top-left corner of the rectangle.
     * @param y The y-coordinate of the top-left corner of the rectangle.
     * @param w The width of the rectangle.
     * @param h The height of the rectangle.
     * @param type The style of the rounded corners, with values ranging from 0 to 4.
     *
     * @return void
     */
    void oxy_FillIndentedRectangle(uint16_t x, uint8_t y, int w, uint8_t h, uint8_t type);

    /**
     * @brief Renders a oxygen style rectangle (Not filled)
     *
     * @param x: X Position
     * @param y: Y Position
     * @param w: Width of Rectangle
     * @param h: Height of Rectangle
     * @param type: ranges 0 one to 4
     *
     * @return void
     */
    void oxy_IndentedRectangle(uint16_t x, uint8_t y, int w, uint8_t h, uint8_t type);

    /**
     * @brief Renders a curve
     *
     * This function uses the Bézier curve algorithm to draw a curve using three
     * control points. The curve passes through the first and last control points,
     * and is influenced by the position of the middle control point.
     *
     * @param x1: The x-coordinate of the first control point.
     * @param y1: The y-coordinate of the first control point.
     * @param x2: The x-coordinate of the middle control point.
     * @param y2: The y-coordinate of the middle control point.
     * @param x3: The x-coordinate of the last control point.
     * @param y3: The y-coordinate of the last control point.
     *
     * @return void
     */
    void oxy_Curve(int x1, int y1, int x2, int y2, int x3, int y3);

    /**
     * @brief Render an arc.
     *
     * This function draws an arc with the specified parameters. The arc is centered at the point (x, y) and has a radius of radius.
     * The arc is drawn from the start_angle to the end_angle in a counterclockwise direction.
     *
     * @param x The x-coordinate of the center of the arc.
     * @param y The y-coordinate of the center of the arc.
     * @param radius The radius of the arc.
     * @param start_angle The starting angle of the arc, in degrees.
     * @param end_angle The ending angle of the arc, in degrees.
     *
     * @return void
     */
    void oxy_Arc(uint16_t x, uint8_t y, uint24_t radius, int start_angle, int end_angle);

    // Sprite Routines
    /**
     * @brief Apply a new palette onto a sprite.
     *
     * @param in_sprite: Sprite you want to apply palette onto.
     * @param map: List of palette map.
     *
     * @return gfx_sprite_t *: A repalettize sprite.
     */
    gfx_sprite_t *oxy_RepalettizeSprite(gfx_sprite_t *in, const uint16_t *palette, const uint8_t palette_size);

    /**
     * @brief Replaces all colors in sprite with new colors.
     *
     * @param in: Sprite containing color
     * @param old_color: The color you want to change
     * @param new_color: The color you want to replace old color
     *
     * @return gfx_sprite_t *: A sprite with replaced color.
     */
    gfx_sprite_t *oxy_ReplaceSpriteColor(gfx_sprite_t *in, const uint8_t old_color, const uint8_t new_color);

    /**
     * @brief  Apply a mask over a sprite.
     *
     * if "mask_sprite" and "in_sprite" don't have the same width and height the "mask_sprite" dimensions will be changed so it
     * does match the "in_sprite" dimensions.
     *
     * @param  in: Sprite you want to apply mask over.
     * @param  mask: Mask sprite that will be applied over "in_sprite".
     * @param  mask_color: Color of area you want to be removed from in.
     * @param  transparent_color: Transparent color of mask.
     *
     * @return @return gfx_sprite_t *: A sprite a transparent mask applied.
     */
    gfx_sprite_t *oxy_ApplyMaskToSprite(gfx_sprite_t *in, gfx_sprite_t *mask, const uint8_t mask_color, const uint8_t transparent_color);

    // Color Routines.
    /**
     * @brief Condense sprite into using a condensed version of xlibc palette.
     *
     * Make sure that you use 'oxy_InsertCondensePalette' to insert the condensed version of the xlib palette before
     * you condense the sprite.
     *
     * @param in: Pointer to an initialized sprite structure
     */
    void oxy_CondenseSprite(gfx_sprite_t *in);

    /**
     * @brief Inserts a condensed version of xlibc into the palette.
     *
     * @param offset: Palette insert offset
     */
    void oxy_InsertCondensePalette(uint8_t offset);

    /**
     * @brief Get the colors that are in a sprite.
     *
     * @param in: Pointer to an initialized sprite structure
     *
     * @returns Returns a pointer to list of colors.
     */
    uint16_t *oxy_GetSpritePalette(gfx_sprite_t *in);

    /**
     * @brief Converts a Rgb1555 to the closet color in the palette.
     *
     * @param color: Color to convert to Palette
     * @param palette: (uint16_t) Pointer to palette to set
     * @param palette_size: Size of the palette
     *
     * @returns (uint8_t) Palette Position.
     */
    uint8_t oxy_Rgb1555toPalette(uint16_t color, const uint16_t *palette, const uint8_t palette_size);

    /**
     * @brief Convert Rgb1555 colors to RBG.
     *
     * @param color: color to convert
     *
     * @returns A pointer to RBG colors.
     */
    uint8_t *oxy_Rgb1555ToRGB(uint16_t color);

    /**
     * @brief Calculates the difference between two colors.
     *
     * This function calculates the difference between two colors using the sRGB color space. The difference is calculated using the following formula:
     * delta = ((R1 - R2)^2 + (G1 - G2)^2 + (B1 - B2)^2)^(1/2)
     * where R1, G1, and B1 are the red, green, and blue components of color1, and R2, G2, and B2 are the red, green, and blue components of color2.
     *
     * @param color1: The first color to compare.
     * @param color2: The second color to compare.
     *
     * @return int: The difference between the two colors.
     */
    int oxy_ColorDifference(uint16_t color1, uint16_t color2);

    /**
     * @brief Darken a palette by a given amount.
     *
     * This function darkens the colors in the given palette by the specified amount. The amount parameter should be
     * a value between 0 and 255, where 0 represents no change to the palette and 255 represents the maximum amount of
     * darkness that can be applied.
     *
     * @param amount The amount to darken the palette by.
     * @param palette A pointer to the palette to darken.
     * @param palette_size The size of the palette.
     *
     * @return A pointer to the modified palette.
     */
    uint16_t *oxy_DarkenPalette(const uint8_t amount, uint16_t *palette, const uint8_t palette_size);

    /**
     * @brief Lighten a given palette by a specified amount.
     *
     * This function takes a palette of colors, represented as an array of 16-bit unsigned integers, and increases the intensity of each color by the specified amount.
     * The amount parameter should be a value between 0 and 255, with 0 representing no change to the intensity of the colors and 255 representing maximum intensity.
     * The palette parameter is a pointer to the array of colors, and the palette_size parameter is the size of the array.
     *
     * @param amount The amount to lighten the palette, with a maximum value of 255.
     * @param palette A pointer to the array of colors to be lightened.
     * @param palette_size The size of the palette array.
     *
     * @return A pointer to the modified palette.
     */
    uint16_t *oxy_LightenPalette(const uint8_t amount, uint16_t *palette, const uint8_t palette_size);

    /**
     * @brief Returns the transparent color of the current palette.
     *
     * This function returns the transparent color of the current palette without modifying it.
     * The transparent color is used to indicate that a pixel should not be drawn.
     *
     * @return uint8_t The transparent color of the current palette.
     */
    uint8_t oxy_ReturnTransparentColor(void);

    /**
     * @brief Sets the text foreground and background colors
     *
     * This function sets the text foreground and background colors using the provided color codes. The fg parameter sets the text color, and the bg parameter sets the background color. The gfx_SetTextTransparentColor function is also called with the bg color to ensure that the text is properly drawn over the background color.
     *
     * @param fg The color code for the text foreground color
     * @param bg The color code for the text background color
     *
     * @return void
     */
    void oxy_SetTextColor(uint8_t fg, uint8_t bg);

    /**
     * @brief Prints a text wrap at a given position and initial line.
     *
     * This function takes in a string of text and prints it out in a word-wrapped format, starting at the specified position and initial line.
     * The width of the text wrap and the maximum number of lines to display can also be specified.
     *
     * @param text The text to be word-wrapped and printed.
     * @param x The x coordinate of the position to start printing at.
     * @param y The y coordinate of the position to start printing at.
     * @param width The width of the text wrap, in pixels.
     * @param max_lines The maximum number of lines to display.
     * @param init_line The initial line to start printing at.
     *
     * @return void
     */
    void oxy_PrintWordWrap(char *text, uint24_t x, uint8_t y, int width, uint8_t max_lines, uint8_t init_line);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif