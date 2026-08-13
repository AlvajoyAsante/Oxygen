/**
 * @file vram16.h
 * @brief Low-level 16bpp VRAM drawing primitives backed by assembly routines.
 */
#ifndef OXYGEN_VRAM16_LOWLEVEL_H
#define OXYGEN_VRAM16_LOWLEVEL_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Clears the low-level VRAM16 drawing state.
 * @param color RGB1555 color value used to clear.
 */
void vram16_Clear(uint16_t color);

/**
 * @brief Writes one pixel directly to 16bpp LCD VRAM.
 * @param x X position in pixels.
 * @param y Y position in pixels.
 * @param color RGB1555 color value to write.
 */
void vram16_SetPixel(uint16_t x, uint8_t y, uint16_t color);

/**
 * @brief Reads one pixel directly from 16bpp LCD VRAM.
 * @param x X position in pixels.
 * @param y Y position in pixels.
 * @return RGB1555 color stored at the requested position.
 */
uint16_t vram16_GetPixel(uint16_t x, uint8_t y);

/**
 * @brief Draws a horizontal line directly in 16bpp LCD VRAM.
 * @param x Start X position.
 * @param y Y position.
 * @param length Line length in pixels.
 * @param color RGB1555 color value to draw.
 */
void vram16_HorizLine(uint16_t x, uint8_t y, uint16_t length, uint16_t color);

/**
 * @brief Draws a vertical line directly in 16bpp LCD VRAM.
 * @param x X position.
 * @param y Start Y position.
 * @param length Line length in pixels.
 * @param color RGB1555 color value to draw.
 */
void vram16_VertLine(uint16_t x, uint8_t y, uint16_t length, uint16_t color);

/**
 * @brief Draws a rectangle outline directly in 16bpp LCD VRAM.
 * @param x Left X position.
 * @param y Top Y position.
 * @param width Rectangle width in pixels.
 * @param height Rectangle height in pixels.
 * @param color RGB1555 color value to draw.
 */
void vram16_Rectangle(uint16_t x, uint8_t y, uint16_t width, uint16_t height, uint16_t color);

/**
 * @brief Draws a filled rectangle directly in 16bpp LCD VRAM.
 * @param x Left X position.
 * @param y Top Y position.
 * @param width Rectangle width in pixels.
 * @param height Rectangle height in pixels.
 * @param color RGB1555 color value to fill with.
 */
void vram16_FillRectangle(uint16_t x, uint8_t y, uint16_t width, uint16_t height, uint16_t color);

/**
 * @brief Fills the full 16bpp LCD VRAM surface with one color.
 * @param color RGB1555 color value used for the fill.
 */
void vram16_FillScreen(uint16_t color);

/**
 * @brief Replaces every matching RGB1555 color across the full LCD VRAM surface.
 * @param old_color RGB1555 color value to replace.
 * @param new_color Replacement RGB1555 color value.
 */
void vram16_ReplaceColor(uint16_t old_color, uint16_t new_color);

#ifdef __cplusplus
}
#endif

#endif
