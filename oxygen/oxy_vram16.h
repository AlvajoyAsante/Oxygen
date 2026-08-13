/**
 * @file oxy_vram16.h
 * @brief High-level 16bpp VRAM graphics API for homescreen-safe rendering.
 */
#ifndef OXY_VRAM16_H
#define OXY_VRAM16_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define OXY_VRAM16_WIDTH 320
#define OXY_VRAM16_HEIGHT 240

typedef struct oxy_vram16_image_t {
    uint16_t width;
    uint16_t height;
    const uint16_t *data;
} oxy_vram16_image_t;

typedef struct oxy_vram16_indexed_image_t {
    uint16_t width;
    uint16_t height;
    const uint8_t *data;
} oxy_vram16_indexed_image_t;

typedef struct oxy_vram16_font_t {
    uint8_t glyph_width;
    uint8_t glyph_height;
    uint8_t first_char;
    uint8_t last_char;
    const uint8_t *glyph_bitmap;
} oxy_vram16_font_t;

/**
 * @brief Behavior when drawing is requested in an unsafe LCD mode.
 */
typedef enum oxy_vram16_safety_policy_t {
    OXY_VRAM16_UNSAFE_NOOP = 0, /**< Skip draw calls and report unsafe mode. */
    OXY_VRAM16_UNSAFE_FORCE      /**< Draw anyway (advanced use). */
} oxy_vram16_safety_policy_t;

/**
 * @brief Last error state tracked by the VRAM16 layer.
 */
typedef enum oxy_vram16_error_t {
    OXY_VRAM16_ERROR_NONE = 0,   /**< No error. */
    OXY_VRAM16_ERROR_UNSAFE_MODE /**< Drawing skipped due to unsafe LCD state. */
} oxy_vram16_error_t;

/* Safety and mode checks */
/**
 * @brief Captures the current LCD state as the expected safe VRAM16 mode.
 */
void oxy_Vram16InitSafetySnapshot(void);

/**
 * @brief Checks whether the current LCD state matches the safe snapshot.
 * @return True if the LCD state matches the captured safe mode.
 */
bool oxy_Vram16CanDrawNow(void);

/**
 * @brief Sets behavior for drawing attempts in unsafe mode.
 * @param policy Unsafe-mode policy.
 */
void oxy_Vram16SetUnsafeModePolicy(oxy_vram16_safety_policy_t policy);

/**
 * @brief Gets current unsafe-mode policy.
 * @return Active unsafe-mode policy.
 */
oxy_vram16_safety_policy_t oxy_Vram16GetUnsafeModePolicy(void);

/**
 * @brief Gets the last VRAM16 error state.
 * @return Last error code.
 */
oxy_vram16_error_t oxy_Vram16GetLastError(void);

/**
 * @brief Clears the last VRAM16 error state.
 */
void oxy_Vram16ClearLastError(void);

/**
 * @brief Sets an external 16bpp shadow buffer as a draw target.
 * @param buffer Pointer to first pixel of a 16bpp RGB1555 buffer.
 * @param width Buffer width in pixels.
 * @param height Buffer height in pixels.
 * @param stride Number of pixels per row in memory.
 * @return True if the buffer was accepted.
 */
bool oxy_Vram16SetShadowBuffer(uint16_t *buffer, uint16_t width, uint8_t height, uint16_t stride);

/**
 * @brief Configures the default VRAM16 buffer using a tightly packed stride.
 * @param buffer Pointer to first pixel of a 16bpp RGB1555 buffer.
 * @param width Buffer width in pixels.
 * @param height Buffer height in pixels.
 * @return True if the buffer was accepted.
 */
bool oxy_Vram16SetBuffer(uint16_t *buffer, uint16_t width, uint8_t height);

/**
 * @brief Configures the default VRAM16 buffer with an explicit stride.
 * @param buffer Pointer to first pixel of a 16bpp RGB1555 buffer.
 * @param width Buffer width in pixels.
 * @param height Buffer height in pixels.
 * @param stride Number of pixels per row in memory.
 * @return True if the buffer was accepted.
 */
bool oxy_Vram16SetBufferStride(uint16_t *buffer, uint16_t width, uint8_t height, uint16_t stride);

/**
 * @brief Allocates and configures an internal VRAM16 off-screen buffer.
 * @param width Buffer width in pixels.
 * @param height Buffer height in pixels.
 * @return True if the buffer was allocated and configured.
 */
bool oxy_Vram16BeginBuffer(uint16_t width, uint8_t height);

/**
 * @brief Allocates and configures an internal VRAM16 off-screen buffer.
 * @param width Buffer width in pixels.
 * @param height Buffer height in pixels.
 * @param stride Number of pixels per row in memory.
 * @return True if the buffer was allocated and configured.
 */
bool oxy_Vram16BeginBufferStride(uint16_t width, uint8_t height, uint16_t stride);

/**
 * @brief Releases the internal VRAM16 off-screen buffer, if one is owned.
 */
void oxy_Vram16EndBuffer(void);

/**
 * @brief Switches VRAM16 drawing to the configured off-screen buffer.
 * @return True if a configured buffer is available.
 */
bool oxy_Vram16SetDrawBuffer(void);

/**
 * @brief Switches VRAM16 drawing back to live LCD VRAM.
 */
void oxy_Vram16SetDrawScreen(void);

/**
 * @brief Enables or disables drawing to the configured shadow buffer.
 * @param enabled True to draw into shadow buffer instead of live VRAM.
 */
void oxy_Vram16EnableShadowBuffer(bool enabled);

/**
 * @brief Checks whether shadow-buffer rendering is currently enabled.
 * @return True when shadow-buffer rendering is enabled.
 */
bool oxy_Vram16IsShadowBufferEnabled(void);

/**
 * @brief Flushes the shadow buffer to LCD VRAM when current LCD mode is safe.
 * @return True if flush succeeded.
 */
bool oxy_Vram16FlushShadowBuffer(void);

/**
 * @brief Flushes a rectangular region from the shadow buffer to LCD VRAM.
 * @param x Left position of the region.
 * @param y Top position of the region.
 * @param width Region width in pixels.
 * @param height Region height in pixels.
 * @return True if flush succeeded.
 */
bool oxy_Vram16FlushShadowBufferRect(uint16_t x, uint8_t y, uint16_t width, uint8_t height);

/**
 * @brief Selects an external 16bpp buffer as the active VRAM16 draw target.
 * @param buffer Pointer to first pixel of a 16bpp RGB1555 buffer.
 * @param width Buffer width in pixels.
 * @param height Buffer height in pixels.
 * @param stride Number of pixels per row in memory.
 * @return True if the buffer was accepted and buffering was enabled.
 */
bool oxy_Vram16UseBuffer(uint16_t *buffer, uint16_t width, uint8_t height, uint16_t stride);

/**
 * @brief Returns VRAM16 drawing to direct LCD VRAM writes.
 */
void oxy_Vram16UseScreen(void);

/**
 * @brief Blits the active VRAM16 buffer to LCD VRAM.
 * @return True if the blit succeeded.
 */
bool oxy_Vram16Blit(void);

/**
 * @brief Blits a rectangular region from the active VRAM16 buffer to LCD VRAM.
 * @param x Left position of the region.
 * @param y Top position of the region.
 * @param width Region width in pixels.
 * @param height Region height in pixels.
 * @return True if the blit succeeded.
 */
bool oxy_Vram16BlitRect(uint16_t x, uint8_t y, uint16_t width, uint8_t height);

/* Drawing state */
/**
 * @brief Sets the active drawing color.
 * @param color RGB1555 color value.
 */
void oxy_Vram16SetColor(uint16_t color);

/**
 * @brief Gets the active drawing color.
 * @return Current RGB1555 drawing color.
 */
uint16_t oxy_Vram16GetColor(void);

/**
 * @brief Sets the active drawing color from the indexed 16-color palette.
 * @param index Palette index [0, 15].
 */
void oxy_Vram16SetColorIndex(uint8_t index);

/**
 * @brief Gets the indexed palette entry used for the active drawing color.
 * @return Palette index [0, 15].
 */
uint8_t oxy_Vram16GetColorIndex(void);

/**
 * @brief Sets the background color used by text helpers.
 * @param color RGB1555 color value.
 */
void oxy_Vram16SetBackColor(uint16_t color);

/**
 * @brief Gets the background color used by text helpers.
 * @return Current RGB1555 background color.
 */
uint16_t oxy_Vram16GetBackColor(void);

/**
 * @brief Sets the active text background color from the indexed 16-color palette.
 * @param index Palette index [0, 15].
 */
void oxy_Vram16SetBackColorIndex(uint8_t index);

/**
 * @brief Gets the indexed palette entry used for text background color.
 * @return Palette index [0, 15].
 */
uint8_t oxy_Vram16GetBackColorIndex(void);

/**
 * @brief Sets the transparent color used by transparent image helpers.
 * @param color RGB1555 transparent color key.
 */
void oxy_Vram16SetTransparentColor(uint16_t color);

/**
 * @brief Gets the transparent color key.
 * @return Current RGB1555 transparent color key.
 */
uint16_t oxy_Vram16GetTransparentColor(void);

/**
 * @brief Sets the transparent color key from the indexed 16-color palette.
 * @param index Palette index [0, 15].
 */
void oxy_Vram16SetTransparentColorIndex(uint8_t index);

/**
 * @brief Gets the indexed palette entry used as transparent color key.
 * @return Palette index [0, 15].
 */
uint8_t oxy_Vram16GetTransparentColorIndex(void);

/**
 * @brief Enables or disables transparency state.
 * @param enabled True to enable transparency-aware operations.
 */
void oxy_Vram16SetTransparentEnabled(bool enabled);

/**
 * @brief Checks transparency state.
 * @return True when transparency-aware operations are enabled.
 */
bool oxy_Vram16IsTransparentEnabled(void);

/**
 * @brief Sets one entry in the active 16-color indexed sprite palette.
 * @param index Palette index [0, 15].
 * @param color RGB1555 palette color.
 */
void oxy_Vram16SetPaletteColor(uint8_t index, uint16_t color);

/**
 * @brief Gets one entry from the active 16-color indexed sprite palette.
 * @param index Palette index [0, 15].
 * @return RGB1555 palette color, or 0 when index is out of range.
 */
uint16_t oxy_Vram16GetPaletteColor(uint8_t index);

/**
 * @brief Sets up to 16 entries of the active indexed sprite palette.
 * @param colors Palette array in RGB1555.
 * @param count Number of entries to apply (clamped to 16).
 */
void oxy_Vram16SetPalette16(const uint16_t *colors, uint8_t count);

/**
 * @brief Copies out the active indexed sprite palette.
 * @param out_colors Destination array.
 * @param count Number of entries to copy (clamped to 16).
 */
void oxy_Vram16GetPalette16(uint16_t *out_colors, uint8_t count);

/* Color editing */
/**
 * @brief Replaces one RGB1555 color with another across the active VRAM16 target.
 * @param old_color RGB1555 color value to replace.
 * @param new_color Replacement RGB1555 color value.
 */
void oxy_Vram16ReplaceColor(uint16_t old_color, uint16_t new_color);

/**
 * @brief Replaces one RGB1555 color with another within a rectangular region.
 * @param x Left X position.
 * @param y Top Y position.
 * @param width Region width in pixels.
 * @param height Region height in pixels.
 * @param old_color RGB1555 color value to replace.
 * @param new_color Replacement RGB1555 color value.
 */
void oxy_Vram16ReplaceColorRect(uint16_t x, uint8_t y, uint16_t width, uint8_t height,
                                uint16_t old_color, uint16_t new_color);

/* Basic primitives */
/**
 * @brief Clears VRAM with a color.
 * @param color RGB1555 fill color.
 */
void oxy_Vram16Clear(uint16_t color);

/**
 * @brief Draws one pixel with the current draw color.
 * @param x X position.
 * @param y Y position.
 */
void oxy_Vram16Pixel(uint16_t x, uint8_t y);

/**
 * @brief Draws one pixel with an explicit color.
 * @param x X position.
 * @param y Y position.
 * @param color RGB1555 pixel color.
 */
void oxy_Vram16PixelColor(uint16_t x, uint8_t y, uint16_t color);

/**
 * @brief Reads a pixel color.
 * @param x X position.
 * @param y Y position.
 * @return RGB1555 color at (x, y).
 */
uint16_t oxy_Vram16ReadPixel(uint16_t x, uint8_t y);

/**
 * @brief Draws a horizontal line with current draw color.
 * @param x Start X position.
 * @param y Y position.
 * @param length Line length in pixels.
 */
void oxy_Vram16HLine(uint16_t x, uint8_t y, uint16_t length);

/**
 * @brief Draws a horizontal line with explicit color.
 * @param x Start X position.
 * @param y Y position.
 * @param length Line length in pixels.
 * @param color RGB1555 line color.
 */
void oxy_Vram16HLineColor(uint16_t x, uint8_t y, uint16_t length, uint16_t color);

/**
 * @brief Draws a vertical line with current draw color.
 * @param x X position.
 * @param y Start Y position.
 * @param length Line length in pixels.
 */
void oxy_Vram16VLine(uint16_t x, uint8_t y, uint16_t length);

/**
 * @brief Draws a vertical line with explicit color.
 * @param x X position.
 * @param y Start Y position.
 * @param length Line length in pixels.
 * @param color RGB1555 line color.
 */
void oxy_Vram16VLineColor(uint16_t x, uint8_t y, uint16_t length, uint16_t color);

/**
 * @brief Draws a line with current draw color.
 * @param x0 Start X position.
 * @param y0 Start Y position.
 * @param x1 End X position.
 * @param y1 End Y position.
 */
void oxy_Vram16Line(int x0, int y0, int x1, int y1);

/**
 * @brief Draws a line with explicit color.
 * @param x0 Start X position.
 * @param y0 Start Y position.
 * @param x1 End X position.
 * @param y1 End Y position.
 * @param color RGB1555 line color.
 */
void oxy_Vram16LineColor(int x0, int y0, int x1, int y1, uint16_t color);

/**
 * @brief Draws a rectangle outline with current draw color.
 * @param x Left X position.
 * @param y Top Y position.
 * @param width Rectangle width.
 * @param height Rectangle height.
 */
void oxy_Vram16Rect(uint16_t x, uint8_t y, uint16_t width, uint16_t height);

/**
 * @brief Draws a rectangle outline with explicit color.
 * @param x Left X position.
 * @param y Top Y position.
 * @param width Rectangle width.
 * @param height Rectangle height.
 * @param color RGB1555 outline color.
 */
void oxy_Vram16RectColor(uint16_t x, uint8_t y, uint16_t width, uint16_t height, uint16_t color);

/**
 * @brief Draws a filled rectangle with current draw color.
 * @param x Left X position.
 * @param y Top Y position.
 * @param width Rectangle width.
 * @param height Rectangle height.
 */
void oxy_Vram16FillRect(uint16_t x, uint8_t y, uint16_t width, uint16_t height);

/**
 * @brief Draws a filled rectangle with explicit color.
 * @param x Left X position.
 * @param y Top Y position.
 * @param width Rectangle width.
 * @param height Rectangle height.
 * @param color RGB1555 fill color.
 */
void oxy_Vram16FillRectColor(uint16_t x, uint8_t y, uint16_t width, uint16_t height, uint16_t color);

/**
 * @brief Draws a circle outline with current draw color.
 * @param cx Center X position.
 * @param cy Center Y position.
 * @param radius Circle radius.
 */
void oxy_Vram16Circle(int cx, int cy, int radius);

/**
 * @brief Draws a circle outline with explicit color.
 * @param cx Center X position.
 * @param cy Center Y position.
 * @param radius Circle radius.
 * @param color RGB1555 outline color.
 */
void oxy_Vram16CircleColor(int cx, int cy, int radius, uint16_t color);

/**
 * @brief Draws a filled circle with current draw color.
 * @param cx Center X position.
 * @param cy Center Y position.
 * @param radius Circle radius.
 */
void oxy_Vram16FillCircle(int cx, int cy, int radius);

/**
 * @brief Draws a filled circle with explicit color.
 * @param cx Center X position.
 * @param cy Center Y position.
 * @param radius Circle radius.
 * @param color RGB1555 fill color.
 */
void oxy_Vram16FillCircleColor(int cx, int cy, int radius, uint16_t color);

/* Image and sprite blits */
/**
 * @brief Draws a full image at destination coordinates.
 * @param image Image source descriptor.
 * @param x Destination X position.
 * @param y Destination Y position.
 */
void oxy_Vram16DrawImage(const oxy_vram16_image_t *image, int x, int y);

/**
 * @brief Draws a full image and skips pixels matching a transparent key.
 * @param image Image source descriptor.
 * @param x Destination X position.
 * @param y Destination Y position.
 * @param transparent_color RGB1555 transparent color key.
 */
void oxy_Vram16DrawImageTransparent(const oxy_vram16_image_t *image, int x, int y, uint16_t transparent_color);

/**
 * @brief Draws a full image and skips pixels matching the active transparent key.
 * @param image Image source descriptor.
 * @param x Destination X position.
 * @param y Destination Y position.
 */
void oxy_Vram16DrawImageTransparentWithState(const oxy_vram16_image_t *image, int x, int y);

/**
 * @brief Draws a sub-rectangle of an image.
 * @param image Image source descriptor.
 * @param src_x Source X position.
 * @param src_y Source Y position.
 * @param src_w Source width.
 * @param src_h Source height.
 * @param dst_x Destination X position.
 * @param dst_y Destination Y position.
 */
void oxy_Vram16DrawSubImage(const oxy_vram16_image_t *image,
                            int src_x, int src_y, int src_w, int src_h,
                            int dst_x, int dst_y);

/**
 * @brief Draws an image scaled with nearest-neighbor sampling.
 * @param image Image source descriptor.
 * @param dst_x Destination X position.
 * @param dst_y Destination Y position.
 * @param dst_w Destination width.
 * @param dst_h Destination height.
 */
void oxy_Vram16DrawImageScaledNearest(const oxy_vram16_image_t *image,
                                      int dst_x, int dst_y,
                                      int dst_w, int dst_h);

/**
 * @brief Draws an image scaled with nearest-neighbor using transparency state.
 * @param image Image source descriptor.
 * @param dst_x Destination X position.
 * @param dst_y Destination Y position.
 * @param dst_w Destination width.
 * @param dst_h Destination height.
 */
void oxy_Vram16DrawImageScaledNearestWithState(const oxy_vram16_image_t *image,
                                               int dst_x, int dst_y,
                                               int dst_w, int dst_h);

/**
 * @brief Draws an image scaled with nearest-neighbor sampling and transparency key.
 * @param image Image source descriptor.
 * @param dst_x Destination X position.
 * @param dst_y Destination Y position.
 * @param dst_w Destination width.
 * @param dst_h Destination height.
 * @param transparent_color RGB1555 transparent color key.
 */
void oxy_Vram16DrawImageScaledNearestTransparent(const oxy_vram16_image_t *image,
                                                 int dst_x, int dst_y,
                                                 int dst_w, int dst_h,
                                                 uint16_t transparent_color);

/**
 * @brief Draws a full indexed image using the active 16-color palette.
 * @param image Indexed image source descriptor.
 * @param x Destination X position.
 * @param y Destination Y position.
 */
void oxy_Vram16DrawIndexedImage(const oxy_vram16_indexed_image_t *image, int x, int y);

/**
 * @brief Draws a full indexed image using the active palette and transparent index.
 * @param image Indexed image source descriptor.
 * @param x Destination X position.
 * @param y Destination Y position.
 * @param transparent_index Palette index treated as transparent.
 */
void oxy_Vram16DrawIndexedImageTransparent(const oxy_vram16_indexed_image_t *image,
                                           int x, int y,
                                           uint8_t transparent_index);

/**
 * @brief Draws an indexed image scaled with nearest-neighbor sampling.
 * @param image Indexed image source descriptor.
 * @param dst_x Destination X position.
 * @param dst_y Destination Y position.
 * @param dst_w Destination width.
 * @param dst_h Destination height.
 */
void oxy_Vram16DrawIndexedImageScaledNearest(const oxy_vram16_indexed_image_t *image,
                                             int dst_x, int dst_y,
                                             int dst_w, int dst_h);

/**
 * @brief Draws an indexed image scaled with nearest-neighbor using transparency state.
 * @param image Indexed image source descriptor.
 * @param dst_x Destination X position.
 * @param dst_y Destination Y position.
 * @param dst_w Destination width.
 * @param dst_h Destination height.
 */
void oxy_Vram16DrawIndexedImageScaledNearestWithState(const oxy_vram16_indexed_image_t *image,
                                                      int dst_x, int dst_y,
                                                      int dst_w, int dst_h);

/**
 * @brief Draws an indexed image scaled with nearest-neighbor and transparent index.
 * @param image Indexed image source descriptor.
 * @param dst_x Destination X position.
 * @param dst_y Destination Y position.
 * @param dst_w Destination width.
 * @param dst_h Destination height.
 * @param transparent_index Palette index treated as transparent.
 */
void oxy_Vram16DrawIndexedImageScaledNearestTransparent(const oxy_vram16_indexed_image_t *image,
                                                        int dst_x, int dst_y,
                                                        int dst_w, int dst_h,
                                                        uint8_t transparent_index);

/* Text and font support */
/**
 * @brief Sets the active font descriptor.
 * @param font Font descriptor to use, or NULL to restore the built-in font.
 */
void oxy_Vram16SetFont(const oxy_vram16_font_t *font);

/**
 * @brief Gets the active font descriptor.
 * @return Current font descriptor, or the built-in default font when none was set.
 */
const oxy_vram16_font_t* oxy_Vram16GetFont(void);

/**
 * @brief Sets the text cursor position used by oxy_Vram16Print.
 * @param x Cursor X position.
 * @param y Cursor Y position.
 */
void oxy_Vram16SetCursor(uint16_t x, uint8_t y);

/**
 * @brief Gets text cursor X position.
 * @return Cursor X position.
 */
uint16_t oxy_Vram16GetCursorX(void);

/**
 * @brief Gets text cursor Y position.
 * @return Cursor Y position.
 */
uint8_t oxy_Vram16GetCursorY(void);

/**
 * @brief Sets horizontal and vertical text scale.
 * @param x_scale Horizontal scale (minimum 1).
 * @param y_scale Vertical scale (minimum 1).
 */
void oxy_Vram16SetTextScale(uint8_t x_scale, uint8_t y_scale);

/**
 * @brief Sets uniform text size for both horizontal and vertical scale.
 * @param size Text size multiplier (minimum 1).
 */
void oxy_Vram16SetTextSize(uint8_t size);

/**
 * @brief Gets the uniform text size.
 * @return Current horizontal text size multiplier.
 * @note If non-uniform scale is used via oxy_Vram16SetTextScale, this returns
 * the X scale component.
 */
uint8_t oxy_Vram16GetTextSize(void);

/**
 * @brief Gets horizontal text scale.
 * @return Horizontal text scale.
 */
uint8_t oxy_Vram16GetTextScaleX(void);

/**
 * @brief Gets vertical text scale.
 * @return Vertical text scale.
 */
uint8_t oxy_Vram16GetTextScaleY(void);

/**
 * @brief Draws one character with current text colors.
 * @param x Destination X position.
 * @param y Destination Y position.
 * @param c Character to draw.
 */
void oxy_Vram16DrawChar(int x, int y, char c);

/**
 * @brief Draws one character with explicit colors.
 * @param x Destination X position.
 * @param y Destination Y position.
 * @param c Character to draw.
 * @param fg Foreground RGB1555 color.
 * @param bg Background RGB1555 color.
 * @param draw_bg True to draw background pixels for off bits.
 */
void oxy_Vram16DrawCharColor(int x, int y, char c, uint16_t fg, uint16_t bg, bool draw_bg);

/**
 * @brief Draws a string with current text colors.
 * @param x Destination X position.
 * @param y Destination Y position.
 * @param text Null-terminated text string.
 */
void oxy_Vram16DrawString(int x, int y, const char *text);

/**
 * @brief Draws a string with explicit colors.
 * @param x Destination X position.
 * @param y Destination Y position.
 * @param text Null-terminated text string.
 * @param fg Foreground RGB1555 color.
 * @param bg Background RGB1555 color.
 * @param draw_bg True to draw background pixels for off bits.
 */
void oxy_Vram16DrawStringColor(int x, int y, const char *text, uint16_t fg, uint16_t bg, bool draw_bg);

/**
 * @brief Prints text at the current cursor and advances cursor position.
 * @param text Null-terminated text string.
 */
void oxy_Vram16Print(const char *text);

#ifdef __cplusplus
}
#endif

#endif
