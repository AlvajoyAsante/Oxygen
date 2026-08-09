/**
 * @file oxy_gfx.h
 * @brief Declares Oxygen drawing, palette, and sprite utility functions.
 */
#ifndef OXY_GFX_H
#define OXY_GFX_H

#include <tice.h>
#include <graphx.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @brief Draws a filled rounded rectangle.
 * @param x X position.
 * @param y Y position.
 * @param w Rectangle width.
 * @param h Rectangle height.
 * @param type Rounded-edge variant from 1 to 4.
 */
void oxy_FillRoundRectangle(uint16_t x, uint8_t y, int w, uint8_t h, uint8_t type);

#define oxy_FillRoundRectangle_Upper(x, y, w, h) \
oxy_FillRoundRectangle(x, y, w, h, 1)

#define oxy_FillRoundRectangle_Bottom(x, y, w, h) \
oxy_FillRoundRectangle(x, y, w, h, 2)

#define oxy_FillRoundRectangle_Left(x, y, w, h) \
oxy_FillRoundRectangle(x, y, w, h, 3)

#define oxy_FillRoundRectangle_Right(x, y, w, h) \
oxy_FillRoundRectangle(x, y, w, h, 4)

/**
 * @brief Draws a rounded rectangle outline.
 * @param x X position.
 * @param y Y position.
 * @param w Rectangle width.
 * @param h Rectangle height.
 * @param type Rounded-edge variant from 1 to 4.
 */
void oxy_RoundRectangle(uint16_t x, uint8_t y, int w, uint8_t h, uint8_t type);

/**
 * @brief Draws a filled rectangle with an outline.
 * @param x X position.
 * @param y Y position.
 * @param width Rectangle width.
 * @param height Rectangle height.
 * @param fill_color Fill color index.
 * @param border_color Border color index.
 */
void oxy_OutlinedRectangle(uint16_t x, uint8_t y, uint16_t width, uint8_t height,
						   uint8_t fill_color, uint8_t border_color);

/**
 * @brief Draws a filled rounded rectangle with an outline.
 * @param x X position.
 * @param y Y position.
 * @param width Rectangle width.
 * @param height Rectangle height.
 * @param fill_color Fill color index.
 * @param border_color Border color index.
 */
void oxy_OutlinedRoundRectangle(uint16_t x, uint8_t y, uint16_t width,
								uint8_t height, uint8_t fill_color,
								uint8_t border_color);

/**
 * @brief Sets the foreground and background text colors.
 * @param foreground Text foreground color index.
 * @param background Text background color index.
 */
void oxy_SetTextColor(uint8_t foreground, uint8_t background);

#define oxy_RoundRectangle_Upper(x, y, w, h) \
oxy_RoundRectangle(x, y, w, h, 1)

#define oxy_RoundRectangle_Bottom(x, y, w, h) \
oxy_RoundRectangle(x, y, w, h, 2)

#define oxy_RoundRectangle_Left(x, y, w, h) \
oxy_RoundRectangle(x, y, w, h, 3)

#define oxy_RoundRectangle_Right(x, y, w, h) \
oxy_RoundRectangle(x, y, w, h, 4)

// Sprite Routines
/**
 * @brief Replaces one palette index throughout a sprite.
 * @param in Source sprite.
 * @param old_color Palette index to replace.
 * @param new_color Replacement palette index.
 * @return Newly allocated sprite with the color replaced.
 */
gfx_sprite_t* oxy_ReplaceSpriteColor(gfx_sprite_t *in, const uint8_t old_color, const uint8_t new_color);

/**
 * @brief Applies a mask sprite over another sprite.
 * @param in Source sprite.
 * @param mask Mask sprite to apply.
 * @param mask_color Palette index in the mask used to trigger replacement.
 * @param transparent_color Palette index treated as transparent.
 * @return Newly allocated masked sprite.
 */
gfx_sprite_t * oxy_ApplyMaskToSprite(gfx_sprite_t *in, gfx_sprite_t *mask, const uint8_t mask_color, const uint8_t transparent_color);

/**
 * @brief Applies a replacement palette to a sprite.
 * @param in Source sprite.
 * @param palette Palette data to apply.
 * @param palette_size Number of palette entries.
 * @return Newly allocated repalettized sprite.
 */
gfx_sprite_t* oxy_RepalettizeSprite(gfx_sprite_t *in, const uint16_t *palette, const uint8_t palette_size);

// Color Routines.
/**
 * @brief Converts a sprite to use the condensed xlibc palette.
 * @param in Pointer to an initialized sprite structure.
 * @return Newly allocated condensed sprite.
 */
gfx_sprite_t* oxy_CondenseSprite(gfx_sprite_t* in);

/**
 * @brief Maps a palette entry to the condensed xlibc palette.
 * @param color Source palette entry.
 * @return Converted condensed palette index.
 */
uint8_t oxy_GetCondenseColor(uint8_t color);

/**
 * @brief Inserts the condensed xlibc palette into the active palette.
 * @param offset Palette index to insert at.
 */
void oxy_InsertCondensePalette(uint8_t offset);

/**
 * @brief Builds a palette list from the colors used by a sprite.
 * @param in Pointer to an initialized sprite structure.
 * @return Pointer to the generated palette list.
 */
uint16_t* oxy_GetSpritePalette(gfx_sprite_t *in);

/**
 * @brief Converts an RGB1555 color to the nearest palette entry.
 * @param color Color to convert.
 * @param palette Palette to search.
 * @param palette_size Number of palette entries.
 * @return Best matching palette position.
 */
uint8_t oxy_Rgb1555ToPalette(uint16_t color, const uint16_t *palette, const uint8_t palette_size);

#define oxy_Rgb1555toGFX(x) oxy_Rgb1555ToPalette(x, gfx_palette, 256)

/**
 * @brief Converts an RGB1555 color to RGB components.
 * @param color Color to convert.
 * @return Pointer to the converted RGB triplet.
 */
uint8_t* oxy_Rgb1555ToRGB(uint16_t color);

/**
 * @brief Computes the visual difference between two RGB1555 colors.
 * @param color1 First color.
 * @param color2 Second color.
 * @return Difference score used by oxy_Rgb1555ToPalette.
 */
int oxy_ColorDifference(uint16_t color1, uint16_t color2);

/**
 * @brief Darkens a rectangular screen region.
 * @param x X position.
 * @param y Y position.
 * @param w Rectangle width.
 * @param h Rectangle height.
 * @param amount Amount to darken, from 0 to 255.
 */
void oxy_DarkenArea(uint16_t x, uint8_t y, int w, uint8_t h, uint8_t amount);

/**
 * @brief Lightens a rectangular screen region.
 * @param x X position.
 * @param y Y position.
 * @param w Rectangle width.
 * @param h Rectangle height.
 * @param amount Amount to lighten, from 0 to 255.
 */
void oxy_LightenArea(uint16_t x, uint8_t y, int w, uint8_t h, uint8_t amount);

/**
 * @brief Returns a darkened copy of a palette.
 * @param amount Amount to darken, from 0 to 255.
 * @param palette Palette to adjust.
 * @param palette_size Number of palette entries.
 * @return Pointer to the darkened palette.
 */
uint16_t* oxy_DarkenPalette(const uint8_t amount, uint16_t *palette, const uint8_t palette_size);

/**
 * @brief Returns a lightened copy of a palette.
 * @param amount Amount to lighten, from 0 to 255.
 * @param palette Palette to adjust.
 * @param palette_size Number of palette entries.
 * @return Pointer to the lightened palette.
 */
uint16_t* oxy_LightenPalette(const uint8_t amount, uint16_t *palette, const uint8_t palette_size);

/**
 * @brief Returns the palette index reserved for transparency.
 * @return Transparent palette color index.
 */
uint8_t oxy_ReturnTransparentColor(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif