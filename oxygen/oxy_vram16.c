#include "oxy_vram16.h"
#include "oxy_gfx.h"
#include "asm/vram16.h"

#include <sys/lcd.h>

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static uint16_t oxy_vram16_color = 0x0000;
static uint16_t oxy_vram16_back_color = 0xFFFF;
static uint16_t oxy_vram16_transparent = 0x0000;
static bool oxy_vram16_transparent_enabled = false;
static uint8_t oxy_vram16_color_index = 0;
static uint8_t oxy_vram16_back_color_index = 15;
static uint8_t oxy_vram16_transparent_index = 0;
static bool oxy_vram16_color_uses_index = false;
static bool oxy_vram16_back_color_uses_index = false;
static bool oxy_vram16_transparent_uses_index = false;
static uint16_t oxy_vram16_palette16[16] = {
    0x0000, 0x0842, 0x1084, 0x18C6,
    0x2108, 0x294A, 0x318C, 0x39CE,
    0x4631, 0x4E73, 0x56B5, 0x5EF7,
    0x6739, 0x6F7B, 0x77BD, 0x7FFF
};
static uint16_t oxy_vram16_cursor_x = 0;
static uint8_t oxy_vram16_cursor_y = 0;
static uint8_t oxy_vram16_scale_x = 1;
static uint8_t oxy_vram16_scale_y = 1;
static const oxy_vram16_font_t *oxy_vram16_font = NULL;
static uint16_t *oxy_vram16_shadow = NULL;
static uint16_t *oxy_vram16_owned_shadow = NULL;
static uint16_t oxy_vram16_shadow_width = 0;
static uint8_t oxy_vram16_shadow_height = 0;
static uint16_t oxy_vram16_shadow_stride = 0;
static bool oxy_vram16_shadow_enabled = false;
static uint32_t oxy_vram16_safe_upbase = 0;
static uint32_t oxy_vram16_safe_lpbase = 0;
static uint32_t oxy_vram16_safe_control = 0;
static bool oxy_vram16_snapshot_valid = false;
static oxy_vram16_safety_policy_t oxy_vram16_policy = OXY_VRAM16_UNSAFE_NOOP;
static oxy_vram16_error_t oxy_vram16_last_error = OXY_VRAM16_ERROR_NONE;
static uint8_t oxy_vram16_default_font_bitmap[(('|' - ' ') + 1) * 7];
static bool oxy_vram16_default_font_initialized = false;

static const oxy_vram16_font_t oxy_vram16_default_font = {
    5,
    7,
    ' ',
    '|',
    oxy_vram16_default_font_bitmap
};

static void oxy_Vram16SyncIndexedState(void)
{
    if (oxy_vram16_color_uses_index) {
        oxy_vram16_color = oxy_vram16_palette16[oxy_vram16_color_index & 0x0F];
    }
    if (oxy_vram16_back_color_uses_index) {
        oxy_vram16_back_color = oxy_vram16_palette16[oxy_vram16_back_color_index & 0x0F];
    }
    if (oxy_vram16_transparent_uses_index) {
        oxy_vram16_transparent = oxy_vram16_palette16[oxy_vram16_transparent_index & 0x0F];
    }
}

static void oxy_Vram16SetDefaultGlyph(char c, const uint8_t rows[7])
{
    uint16_t index = (uint16_t)(c - ' ') * 7;
    for (uint8_t row = 0; row < 7; row++) {
        oxy_vram16_default_font_bitmap[index + row] = rows[row];
    }
}

static void oxy_Vram16InitDefaultFont(void)
{
    static const uint8_t glyph_0[7] = {0x70, 0x88, 0x98, 0xA8, 0xC8, 0x88, 0x70};
    static const uint8_t glyph_1[7] = {0x20, 0x60, 0x20, 0x20, 0x20, 0x20, 0x70};
    static const uint8_t glyph_2[7] = {0x70, 0x88, 0x08, 0x10, 0x20, 0x40, 0xF8};
    static const uint8_t glyph_3[7] = {0xF0, 0x08, 0x08, 0x70, 0x08, 0x08, 0xF0};
    static const uint8_t glyph_4[7] = {0x10, 0x30, 0x50, 0x90, 0xF8, 0x10, 0x10};
    static const uint8_t glyph_5[7] = {0xF8, 0x80, 0x80, 0xF0, 0x08, 0x08, 0xF0};
    static const uint8_t glyph_6[7] = {0x70, 0x80, 0x80, 0xF0, 0x88, 0x88, 0x70};
    static const uint8_t glyph_7[7] = {0xF8, 0x08, 0x10, 0x20, 0x20, 0x20, 0x20};
    static const uint8_t glyph_8[7] = {0x70, 0x88, 0x88, 0x70, 0x88, 0x88, 0x70};
    static const uint8_t glyph_9[7] = {0x70, 0x88, 0x88, 0x78, 0x08, 0x08, 0x70};
    static const uint8_t glyph_a[7] = {0x70, 0x88, 0x88, 0xF8, 0x88, 0x88, 0x88};
    static const uint8_t glyph_b[7] = {0xF0, 0x88, 0x88, 0xF0, 0x88, 0x88, 0xF0};
    static const uint8_t glyph_c[7] = {0x70, 0x88, 0x80, 0x80, 0x80, 0x88, 0x70};
    static const uint8_t glyph_d[7] = {0xE0, 0x90, 0x88, 0x88, 0x88, 0x90, 0xE0};
    static const uint8_t glyph_e[7] = {0xF8, 0x80, 0x80, 0xF0, 0x80, 0x80, 0xF8};
    static const uint8_t glyph_f[7] = {0xF8, 0x80, 0x80, 0xF0, 0x80, 0x80, 0x80};
    static const uint8_t glyph_g[7] = {0x70, 0x88, 0x80, 0xB8, 0x88, 0x88, 0x70};
    static const uint8_t glyph_h[7] = {0x88, 0x88, 0x88, 0xF8, 0x88, 0x88, 0x88};
    static const uint8_t glyph_i[7] = {0xF8, 0x20, 0x20, 0x20, 0x20, 0x20, 0xF8};
    static const uint8_t glyph_j[7] = {0x38, 0x10, 0x10, 0x10, 0x10, 0x90, 0x60};
    static const uint8_t glyph_k[7] = {0x88, 0x90, 0xA0, 0xC0, 0xA0, 0x90, 0x88};
    static const uint8_t glyph_l[7] = {0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xF8};
    static const uint8_t glyph_m[7] = {0x88, 0xD8, 0xA8, 0xA8, 0x88, 0x88, 0x88};
    static const uint8_t glyph_n[7] = {0x88, 0xC8, 0xA8, 0x98, 0x88, 0x88, 0x88};
    static const uint8_t glyph_o[7] = {0x70, 0x88, 0x88, 0x88, 0x88, 0x88, 0x70};
    static const uint8_t glyph_p[7] = {0xF0, 0x88, 0x88, 0xF0, 0x80, 0x80, 0x80};
    static const uint8_t glyph_q[7] = {0x70, 0x88, 0x88, 0x88, 0xA8, 0x90, 0x68};
    static const uint8_t glyph_r[7] = {0xF0, 0x88, 0x88, 0xF0, 0xA0, 0x90, 0x88};
    static const uint8_t glyph_s[7] = {0x78, 0x80, 0x80, 0x70, 0x08, 0x08, 0xF0};
    static const uint8_t glyph_t[7] = {0xF8, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20};
    static const uint8_t glyph_u[7] = {0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x70};
    static const uint8_t glyph_v[7] = {0x88, 0x88, 0x88, 0x88, 0x88, 0x50, 0x20};
    static const uint8_t glyph_w[7] = {0x88, 0x88, 0x88, 0xA8, 0xA8, 0xA8, 0x50};
    static const uint8_t glyph_x[7] = {0x88, 0x50, 0x20, 0x20, 0x20, 0x50, 0x88};
    static const uint8_t glyph_y[7] = {0x88, 0x88, 0x50, 0x20, 0x20, 0x20, 0x20};
    static const uint8_t glyph_z[7] = {0xF8, 0x08, 0x10, 0x20, 0x40, 0x80, 0xF8};
    static const uint8_t glyph_period[7] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30};
    static const uint8_t glyph_plus[7] = {0x00, 0x20, 0x20, 0xF8, 0x20, 0x20, 0x00};
    static const uint8_t glyph_equals[7] = {0x00, 0x00, 0xF8, 0x00, 0xF8, 0x00, 0x00};
    static const uint8_t glyph_greater[7] = {0x80, 0x40, 0x20, 0x10, 0x20, 0x40, 0x80};
    static const uint8_t glyph_colon[7] = {0x00, 0x30, 0x30, 0x00, 0x30, 0x30, 0x00};
    static const uint8_t glyph_semicolon[7] = {0x00, 0x30, 0x30, 0x00, 0x30, 0x20, 0x40};
    static const uint8_t glyph_slash[7] = {0x08, 0x10, 0x10, 0x20, 0x20, 0x40, 0x80};
    static const uint8_t glyph_backslash[7] = {0x80, 0x40, 0x40, 0x20, 0x20, 0x10, 0x08};
    static const uint8_t glyph_bar[7] = {0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20};
    static const uint8_t glyph_dash[7] = {0x00, 0x00, 0x00, 0xF8, 0x00, 0x00, 0x00};
    static const uint8_t glyph_space[7] = {0, 0, 0, 0, 0, 0, 0};

    if (oxy_vram16_default_font_initialized) return;

    memset(oxy_vram16_default_font_bitmap, 0, sizeof(oxy_vram16_default_font_bitmap));
    oxy_Vram16SetDefaultGlyph(' ', glyph_space);
    oxy_Vram16SetDefaultGlyph('0', glyph_0);
    oxy_Vram16SetDefaultGlyph('1', glyph_1);
    oxy_Vram16SetDefaultGlyph('2', glyph_2);
    oxy_Vram16SetDefaultGlyph('3', glyph_3);
    oxy_Vram16SetDefaultGlyph('4', glyph_4);
    oxy_Vram16SetDefaultGlyph('5', glyph_5);
    oxy_Vram16SetDefaultGlyph('6', glyph_6);
    oxy_Vram16SetDefaultGlyph('7', glyph_7);
    oxy_Vram16SetDefaultGlyph('8', glyph_8);
    oxy_Vram16SetDefaultGlyph('9', glyph_9);
    oxy_Vram16SetDefaultGlyph('A', glyph_a);
    oxy_Vram16SetDefaultGlyph('B', glyph_b);
    oxy_Vram16SetDefaultGlyph('C', glyph_c);
    oxy_Vram16SetDefaultGlyph('D', glyph_d);
    oxy_Vram16SetDefaultGlyph('E', glyph_e);
    oxy_Vram16SetDefaultGlyph('F', glyph_f);
    oxy_Vram16SetDefaultGlyph('G', glyph_g);
    oxy_Vram16SetDefaultGlyph('H', glyph_h);
    oxy_Vram16SetDefaultGlyph('I', glyph_i);
    oxy_Vram16SetDefaultGlyph('J', glyph_j);
    oxy_Vram16SetDefaultGlyph('K', glyph_k);
    oxy_Vram16SetDefaultGlyph('L', glyph_l);
    oxy_Vram16SetDefaultGlyph('M', glyph_m);
    oxy_Vram16SetDefaultGlyph('N', glyph_n);
    oxy_Vram16SetDefaultGlyph('O', glyph_o);
    oxy_Vram16SetDefaultGlyph('P', glyph_p);
    oxy_Vram16SetDefaultGlyph('Q', glyph_q);
    oxy_Vram16SetDefaultGlyph('R', glyph_r);
    oxy_Vram16SetDefaultGlyph('S', glyph_s);
    oxy_Vram16SetDefaultGlyph('T', glyph_t);
    oxy_Vram16SetDefaultGlyph('U', glyph_u);
    oxy_Vram16SetDefaultGlyph('V', glyph_v);
    oxy_Vram16SetDefaultGlyph('W', glyph_w);
    oxy_Vram16SetDefaultGlyph('X', glyph_x);
    oxy_Vram16SetDefaultGlyph('Y', glyph_y);
    oxy_Vram16SetDefaultGlyph('Z', glyph_z);
    oxy_Vram16SetDefaultGlyph('.', glyph_period);
    oxy_Vram16SetDefaultGlyph('+', glyph_plus);
    oxy_Vram16SetDefaultGlyph('=', glyph_equals);
    oxy_Vram16SetDefaultGlyph('>', glyph_greater);
    oxy_Vram16SetDefaultGlyph(':', glyph_colon);
    oxy_Vram16SetDefaultGlyph(';', glyph_semicolon);
    oxy_Vram16SetDefaultGlyph('/', glyph_slash);
    oxy_Vram16SetDefaultGlyph('\\', glyph_backslash);
    oxy_Vram16SetDefaultGlyph('|', glyph_bar);
    oxy_Vram16SetDefaultGlyph('-', glyph_dash);
    oxy_vram16_default_font_initialized = true;
}

static const oxy_vram16_font_t *oxy_Vram16ResolveFont(void)
{
    oxy_Vram16InitDefaultFont();
    return oxy_vram16_font ? oxy_vram16_font : &oxy_vram16_default_font;
}

static void oxy_Vram16ReleaseOwnedBuffer(void)
{
    if (!oxy_vram16_owned_shadow) return;

    free(oxy_vram16_owned_shadow);
    if (oxy_vram16_shadow == oxy_vram16_owned_shadow) {
        oxy_vram16_shadow = NULL;
        oxy_vram16_shadow_width = 0;
        oxy_vram16_shadow_height = 0;
        oxy_vram16_shadow_stride = 0;
        oxy_vram16_shadow_enabled = false;
    }
    oxy_vram16_owned_shadow = NULL;
}

static void oxy_Vram16EnsureSafetySnapshot(void)
{
    if (oxy_vram16_snapshot_valid) return;

    oxy_vram16_safe_upbase = lcd_UpBaseFull;
    oxy_vram16_safe_lpbase = lcd_LpBaseFull;
    oxy_vram16_safe_control = lcd_Control;
    oxy_vram16_snapshot_valid = true;
}

static bool oxy_Vram16ModeMatchesSnapshot(void)
{
    oxy_Vram16EnsureSafetySnapshot();

    return lcd_UpBaseFull == oxy_vram16_safe_upbase &&
           lcd_LpBaseFull == oxy_vram16_safe_lpbase &&
           lcd_Control == oxy_vram16_safe_control;
}

static bool oxy_Vram16Guard(bool mutating)
{
    if (!mutating) {
        if (oxy_Vram16ModeMatchesSnapshot()) {
            oxy_vram16_last_error = OXY_VRAM16_ERROR_NONE;
            return true;
        }
        oxy_vram16_last_error = OXY_VRAM16_ERROR_UNSAFE_MODE;
        return false;
    }

    if (oxy_vram16_policy == OXY_VRAM16_UNSAFE_FORCE) {
        oxy_vram16_last_error = OXY_VRAM16_ERROR_NONE;
        return true;
    }

    if (oxy_Vram16ModeMatchesSnapshot()) {
        oxy_vram16_last_error = OXY_VRAM16_ERROR_NONE;
        return true;
    }

    oxy_vram16_last_error = OXY_VRAM16_ERROR_UNSAFE_MODE;
    return false;
}

void oxy_Vram16InitSafetySnapshot(void)
{
    oxy_vram16_safe_upbase = lcd_UpBaseFull;
    oxy_vram16_safe_lpbase = lcd_LpBaseFull;
    oxy_vram16_safe_control = lcd_Control;
    oxy_vram16_snapshot_valid = true;
    oxy_vram16_last_error = OXY_VRAM16_ERROR_NONE;
}

bool oxy_Vram16CanDrawNow(void)
{
    bool safe = oxy_Vram16ModeMatchesSnapshot();
    oxy_vram16_last_error = safe ? OXY_VRAM16_ERROR_NONE : OXY_VRAM16_ERROR_UNSAFE_MODE;
    return safe;
}

void oxy_Vram16SetUnsafeModePolicy(oxy_vram16_safety_policy_t policy)
{
    oxy_vram16_policy = policy;
}

oxy_vram16_safety_policy_t oxy_Vram16GetUnsafeModePolicy(void)
{
    return oxy_vram16_policy;
}

oxy_vram16_error_t oxy_Vram16GetLastError(void)
{
    return oxy_vram16_last_error;
}

void oxy_Vram16ClearLastError(void)
{
    oxy_vram16_last_error = OXY_VRAM16_ERROR_NONE;
}

bool oxy_Vram16SetShadowBuffer(uint16_t *buffer, uint16_t width, uint8_t height, uint16_t stride)
{
    if (!buffer || width == 0 || height == 0 || stride < width) {
        return false;
    }

    if (oxy_vram16_owned_shadow && buffer != oxy_vram16_owned_shadow) {
        oxy_Vram16ReleaseOwnedBuffer();
    }

    oxy_vram16_shadow = buffer;
    oxy_vram16_shadow_width = width;
    oxy_vram16_shadow_height = height;
    oxy_vram16_shadow_stride = stride;
    return true;
}

bool oxy_Vram16SetBuffer(uint16_t *buffer, uint16_t width, uint8_t height)
{
    return oxy_Vram16SetShadowBuffer(buffer, width, height, width);
}

bool oxy_Vram16SetBufferStride(uint16_t *buffer, uint16_t width, uint8_t height, uint16_t stride)
{
    return oxy_Vram16SetShadowBuffer(buffer, width, height, stride);
}

bool oxy_Vram16BeginBuffer(uint16_t width, uint8_t height)
{
    return oxy_Vram16BeginBufferStride(width, height, width);
}

bool oxy_Vram16BeginBufferStride(uint16_t width, uint8_t height, uint16_t stride)
{
    size_t pixels;
    uint16_t *buffer;

    if (width == 0 || height == 0 || stride < width) {
        return false;
    }

    oxy_Vram16ReleaseOwnedBuffer();
    pixels = (size_t)stride * height;
    buffer = malloc(pixels * sizeof(uint16_t));
    if (!buffer) {
        return false;
    }

    oxy_vram16_owned_shadow = buffer;
    if (!oxy_Vram16SetShadowBuffer(buffer, width, height, stride)) {
        oxy_Vram16ReleaseOwnedBuffer();
        return false;
    }

    oxy_vram16_last_error = OXY_VRAM16_ERROR_NONE;
    return true;
}

void oxy_Vram16EndBuffer(void)
{
    oxy_Vram16ReleaseOwnedBuffer();
    oxy_vram16_last_error = OXY_VRAM16_ERROR_NONE;
}

bool oxy_Vram16SetDrawBuffer(void)
{
    if (!oxy_vram16_shadow) {
        return false;
    }

    oxy_Vram16EnableShadowBuffer(true);
    oxy_vram16_last_error = OXY_VRAM16_ERROR_NONE;
    return true;
}

void oxy_Vram16SetDrawScreen(void)
{
    oxy_Vram16UseScreen();
}

bool oxy_Vram16UseBuffer(uint16_t *buffer, uint16_t width, uint8_t height, uint16_t stride)
{
    if (!oxy_Vram16SetShadowBuffer(buffer, width, height, stride)) {
        return false;
    }

    oxy_Vram16EnableShadowBuffer(true);
    oxy_vram16_last_error = OXY_VRAM16_ERROR_NONE;
    return true;
}

void oxy_Vram16EnableShadowBuffer(bool enabled)
{
    oxy_vram16_shadow_enabled = enabled;
}

void oxy_Vram16UseScreen(void)
{
    oxy_Vram16EnableShadowBuffer(false);
    oxy_vram16_last_error = OXY_VRAM16_ERROR_NONE;
}

bool oxy_Vram16IsShadowBufferEnabled(void)
{
    return oxy_vram16_shadow_enabled;
}

bool oxy_Vram16FlushShadowBufferRect(uint16_t x, uint8_t y, uint16_t width, uint8_t height)
{
    uint16_t copy_w;
    uint8_t copy_h;
    uint16_t clamped_x;
    uint8_t clamped_y;
    uint16_t *vram;

    if (!oxy_vram16_shadow || !oxy_vram16_shadow_enabled) return false;
    if (!oxy_Vram16Guard(true)) return false;
    if (width == 0 || height == 0) {
        oxy_vram16_last_error = OXY_VRAM16_ERROR_NONE;
        return true;
    }

    clamped_x = x;
    clamped_y = y;
    if (clamped_x >= oxy_vram16_shadow_width || clamped_y >= oxy_vram16_shadow_height) {
        oxy_vram16_last_error = OXY_VRAM16_ERROR_NONE;
        return true;
    }

    copy_w = width;
    if ((uint32_t)clamped_x + copy_w > oxy_vram16_shadow_width) {
        copy_w = (uint16_t)(oxy_vram16_shadow_width - clamped_x);
    }
    if ((uint32_t)clamped_x + copy_w > OXY_VRAM16_WIDTH) {
        copy_w = (uint16_t)(OXY_VRAM16_WIDTH - clamped_x);
    }

    copy_h = height;
    if ((uint32_t)clamped_y + copy_h > oxy_vram16_shadow_height) {
        copy_h = (uint8_t)(oxy_vram16_shadow_height - clamped_y);
    }
    if ((uint32_t)clamped_y + copy_h > OXY_VRAM16_HEIGHT) {
        copy_h = (uint8_t)(OXY_VRAM16_HEIGHT - clamped_y);
    }

    if (copy_w == 0 || copy_h == 0) {
        oxy_vram16_last_error = OXY_VRAM16_ERROR_NONE;
        return true;
    }

    vram = (uint16_t *)0xD40000;

    if (clamped_x == 0 && clamped_y == 0 &&
        copy_w == OXY_VRAM16_WIDTH &&
        copy_h == OXY_VRAM16_HEIGHT &&
        oxy_vram16_shadow_stride == OXY_VRAM16_WIDTH) {
        memcpy(vram, oxy_vram16_shadow, (size_t)copy_w * copy_h * sizeof(uint16_t));
        oxy_vram16_last_error = OXY_VRAM16_ERROR_NONE;
        return true;
    }

    for (uint8_t row = 0; row < copy_h; row++) {
        memcpy(vram + (uint16_t)(clamped_y + row) * OXY_VRAM16_WIDTH + clamped_x,
               oxy_vram16_shadow + (uint16_t)(clamped_y + row) * oxy_vram16_shadow_stride + clamped_x,
               (size_t)copy_w * sizeof(uint16_t));
    }

    oxy_vram16_last_error = OXY_VRAM16_ERROR_NONE;

    return true;
}

bool oxy_Vram16FlushShadowBuffer(void)
{
    return oxy_Vram16FlushShadowBufferRect(0, 0, oxy_vram16_shadow_width, oxy_vram16_shadow_height);
}

bool oxy_Vram16Blit(void)
{
    return oxy_Vram16FlushShadowBuffer();
}

bool oxy_Vram16BlitRect(uint16_t x, uint8_t y, uint16_t width, uint8_t height)
{
    return oxy_Vram16FlushShadowBufferRect(x, y, width, height);
}

static bool oxy_Vram16PutPixelTarget(uint16_t x, uint8_t y, uint16_t color)
{
    if (x >= OXY_VRAM16_WIDTH || y >= OXY_VRAM16_HEIGHT) return false;

    if (oxy_vram16_shadow_enabled && oxy_vram16_shadow) {
        if (x >= oxy_vram16_shadow_width || y >= oxy_vram16_shadow_height) return false;
        oxy_vram16_shadow[(uint16_t)y * oxy_vram16_shadow_stride + x] = color;
        oxy_vram16_last_error = OXY_VRAM16_ERROR_NONE;
        return true;
    }

    if (!oxy_Vram16Guard(true)) return false;
    oxy_Vram16SetPixel(x, y, color);
    return true;
}

static bool oxy_Vram16GetPixelTarget(uint16_t x, uint8_t y, uint16_t *out)
{
    if (!out) return false;
    if (x >= OXY_VRAM16_WIDTH || y >= OXY_VRAM16_HEIGHT) return false;

    if (oxy_vram16_shadow_enabled && oxy_vram16_shadow) {
        if (x >= oxy_vram16_shadow_width || y >= oxy_vram16_shadow_height) return false;
        *out = oxy_vram16_shadow[(uint16_t)y * oxy_vram16_shadow_stride + x];
        oxy_vram16_last_error = OXY_VRAM16_ERROR_NONE;
        return true;
    }

    if (!oxy_Vram16Guard(false)) return false;
    *out = oxy_Vram16GetPixel(x, y);
    return true;
}

static bool oxy_Vram16Inside(int x, int y)
{
    return x >= 0 && x < OXY_VRAM16_WIDTH && y >= 0 && y < OXY_VRAM16_HEIGHT;
}

static int oxy_Vram16Abs(int v)
{
    return v < 0 ? -v : v;
}

static void oxy_Vram16FallbackGlyph(int x, int y, unsigned char c,
                                    uint16_t fg, uint16_t bg, bool draw_bg)
{
    int w = 6 * oxy_vram16_scale_x;
    int h = 8 * oxy_vram16_scale_y;

    if (draw_bg) {
        oxy_Vram16FillRectColor((uint16_t)x, (uint8_t)y, (uint16_t)w, (uint16_t)h, bg);
    }

    /* Draw a simple framed glyph with an identifying bit strip. */
    oxy_Vram16RectColor((uint16_t)x, (uint8_t)y, (uint16_t)w, (uint16_t)h, fg);

    for (int bit = 0; bit < 6; bit++) {
        if ((c >> bit) & 1) {
            int px = x + 1 + bit * oxy_vram16_scale_x;
            int py = y + (h / 2);
            oxy_Vram16FillRectColor((uint16_t)px, (uint8_t)py,
                                    oxy_vram16_scale_x, oxy_vram16_scale_y, fg);
        }
    }
}

static void oxy_Vram16DrawGlyphBitmap(int x, int y, unsigned char c,
                                      uint16_t fg, uint16_t bg, bool draw_bg)
{
    const oxy_vram16_font_t *font;
    uint8_t gw;
    uint8_t gh;
    uint8_t first;
    uint8_t last;
    const uint8_t *bitmap;
    uint16_t glyph_index;
    uint8_t bytes_per_row;
    uint16_t glyph_stride;

    font = oxy_Vram16ResolveFont();
    gw = font->glyph_width;
    gh = font->glyph_height;
    first = font->first_char;
    last = font->last_char;

    if (c >= 'a' && c <= 'z' && (uint8_t)(c - 'a' + 'A') >= first && (uint8_t)(c - 'a' + 'A') <= last) {
        c = (unsigned char)(c - 'a' + 'A');
    }

    if (gw == 0 || gh == 0 || c < first || c > last) {
        oxy_Vram16FallbackGlyph(x, y, c, fg, bg, draw_bg);
        return;
    }

    bitmap = font->glyph_bitmap;
    glyph_index = (uint16_t)(c - first);
    bytes_per_row = (uint8_t)((gw + 7) >> 3);
    glyph_stride = (uint16_t)bytes_per_row * gh;

    for (uint8_t row = 0; row < gh; row++) {
        for (uint8_t col = 0; col < gw; col++) {
            const uint8_t *row_ptr = bitmap + glyph_index * glyph_stride + row * bytes_per_row;
            uint8_t mask = (uint8_t)(0x80u >> (col & 7));
            bool on = (row_ptr[col >> 3] & mask) != 0;
            int px = x + col * oxy_vram16_scale_x;
            int py = y + row * oxy_vram16_scale_y;

            if (on) {
                oxy_Vram16FillRectColor((uint16_t)px, (uint8_t)py,
                                        oxy_vram16_scale_x, oxy_vram16_scale_y, fg);
            } else if (draw_bg) {
                oxy_Vram16FillRectColor((uint16_t)px, (uint8_t)py,
                                        oxy_vram16_scale_x, oxy_vram16_scale_y, bg);
            }
        }
    }
}

void oxy_Vram16SetColor(uint16_t color)
{
    oxy_vram16_color_uses_index = false;
    oxy_vram16_color = color;
}

uint16_t oxy_Vram16GetColor(void)
{
    return oxy_vram16_color;
}

void oxy_Vram16SetColorIndex(uint8_t index)
{
    oxy_vram16_color_index = (uint8_t)(index & 0x0F);
    oxy_vram16_color_uses_index = true;
    oxy_vram16_color = oxy_vram16_palette16[oxy_vram16_color_index];
}

uint8_t oxy_Vram16GetColorIndex(void)
{
    return (uint8_t)(oxy_vram16_color_index & 0x0F);
}

void oxy_Vram16SetBackColor(uint16_t color)
{
    oxy_vram16_back_color_uses_index = false;
    oxy_vram16_back_color = color;
}

uint16_t oxy_Vram16GetBackColor(void)
{
    return oxy_vram16_back_color;
}

void oxy_Vram16SetBackColorIndex(uint8_t index)
{
    oxy_vram16_back_color_index = (uint8_t)(index & 0x0F);
    oxy_vram16_back_color_uses_index = true;
    oxy_vram16_back_color = oxy_vram16_palette16[oxy_vram16_back_color_index];
}

uint8_t oxy_Vram16GetBackColorIndex(void)
{
    return (uint8_t)(oxy_vram16_back_color_index & 0x0F);
}

void oxy_Vram16SetTransparentColor(uint16_t color)
{
    oxy_vram16_transparent_uses_index = false;
    oxy_vram16_transparent = color;
}

uint16_t oxy_Vram16GetTransparentColor(void)
{
    return oxy_vram16_transparent;
}

void oxy_Vram16SetTransparentColorIndex(uint8_t index)
{
    oxy_vram16_transparent_index = (uint8_t)(index & 0x0F);
    oxy_vram16_transparent_uses_index = true;
    oxy_vram16_transparent = oxy_vram16_palette16[oxy_vram16_transparent_index];
}

uint8_t oxy_Vram16GetTransparentColorIndex(void)
{
    return (uint8_t)(oxy_vram16_transparent_index & 0x0F);
}

void oxy_Vram16SetTransparentEnabled(bool enabled)
{
    oxy_vram16_transparent_enabled = enabled;
}

bool oxy_Vram16IsTransparentEnabled(void)
{
    return oxy_vram16_transparent_enabled;
}

void oxy_Vram16SetPaletteColor(uint8_t index, uint16_t color)
{
    if (index >= 16) return;
    oxy_vram16_palette16[index] = color;
    oxy_Vram16SyncIndexedState();
}

uint16_t oxy_Vram16GetPaletteColor(uint8_t index)
{
    if (index >= 16) return 0;
    return oxy_vram16_palette16[index];
}

void oxy_Vram16SetPalette16(const uint16_t *colors, uint8_t count)
{
    if (!colors) return;
    if (count > 16) count = 16;
    for (uint8_t i = 0; i < count; i++) {
        oxy_vram16_palette16[i] = colors[i];
    }
    oxy_Vram16SyncIndexedState();
}

void oxy_Vram16GetPalette16(uint16_t *out_colors, uint8_t count)
{
    if (!out_colors) return;
    if (count > 16) count = 16;
    for (uint8_t i = 0; i < count; i++) {
        out_colors[i] = oxy_vram16_palette16[i];
    }
}

void oxy_Vram16Clear(uint16_t color)
{
    if (oxy_vram16_shadow_enabled && oxy_vram16_shadow) {
        uint16_t w = oxy_vram16_shadow_width;
        uint8_t h = oxy_vram16_shadow_height;
        for (uint8_t y = 0; y < h; y++) {
            for (uint16_t x = 0; x < w; x++) {
                oxy_vram16_shadow[(uint16_t)y * oxy_vram16_shadow_stride + x] = color;
            }
        }
        oxy_vram16_last_error = OXY_VRAM16_ERROR_NONE;
        return;
    }

    if (!oxy_Vram16Guard(true)) return;
    oxy_Vram16FillScreen(color);
}

void oxy_Vram16Pixel(uint16_t x, uint8_t y)
{
    (void)oxy_Vram16PutPixelTarget(x, y, oxy_vram16_color);
}

void oxy_Vram16PixelColor(uint16_t x, uint8_t y, uint16_t color)
{
    (void)oxy_Vram16PutPixelTarget(x, y, color);
}

uint16_t oxy_Vram16ReadPixel(uint16_t x, uint8_t y)
{
    uint16_t value = 0;
    if (!oxy_Vram16GetPixelTarget(x, y, &value)) return 0;
    return value;
}

void oxy_Vram16HLine(uint16_t x, uint8_t y, uint16_t length)
{
    oxy_Vram16HLineColor(x, y, length, oxy_vram16_color);
}

void oxy_Vram16HLineColor(uint16_t x, uint8_t y, uint16_t length, uint16_t color)
{
    uint16_t clamped;

    if (!(oxy_vram16_shadow_enabled && oxy_vram16_shadow) && !oxy_Vram16Guard(true)) return;
    if (y >= OXY_VRAM16_HEIGHT || x >= OXY_VRAM16_WIDTH || length == 0) return;
    clamped = length;
    if ((uint32_t)x + clamped > OXY_VRAM16_WIDTH) {
        clamped = (uint16_t)(OXY_VRAM16_WIDTH - x);
    }
    if (oxy_vram16_shadow_enabled && oxy_vram16_shadow) {
        for (uint16_t i = 0; i < clamped; i++) {
            oxy_Vram16PutPixelTarget((uint16_t)(x + i), y, color);
        }
    } else {
        oxy_Vram16HorizLine(x, y, clamped, color);
    }
}

void oxy_Vram16VLine(uint16_t x, uint8_t y, uint16_t length)
{
    oxy_Vram16VLineColor(x, y, length, oxy_vram16_color);
}

void oxy_Vram16VLineColor(uint16_t x, uint8_t y, uint16_t length, uint16_t color)
{
    uint16_t clamped;

    if (!(oxy_vram16_shadow_enabled && oxy_vram16_shadow) && !oxy_Vram16Guard(true)) return;
    if (x >= OXY_VRAM16_WIDTH || y >= OXY_VRAM16_HEIGHT || length == 0) return;
    clamped = length;
    if ((uint32_t)y + clamped > OXY_VRAM16_HEIGHT) {
        clamped = (uint16_t)(OXY_VRAM16_HEIGHT - y);
    }
    if (oxy_vram16_shadow_enabled && oxy_vram16_shadow) {
        for (uint16_t i = 0; i < clamped; i++) {
            oxy_Vram16PutPixelTarget(x, (uint8_t)(y + i), color);
        }
    } else {
        oxy_Vram16VertLine(x, y, clamped, color);
    }
}

void oxy_Vram16Line(int x0, int y0, int x1, int y1)
{
    oxy_Vram16LineColor(x0, y0, x1, y1, oxy_vram16_color);
}

void oxy_Vram16LineColor(int x0, int y0, int x1, int y1, uint16_t color)
{
    int dx = oxy_Vram16Abs(x1 - x0);
    int sx = x0 < x1 ? 1 : -1;
    int dy = -oxy_Vram16Abs(y1 - y0);
    int sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;

    if (!(oxy_vram16_shadow_enabled && oxy_vram16_shadow) && !oxy_Vram16Guard(true)) return;

    while (1) {
        if (oxy_Vram16Inside(x0, y0)) {
            oxy_Vram16PutPixelTarget((uint16_t)x0, (uint8_t)y0, color);
        }
        if (x0 == x1 && y0 == y1) break;

        int e2 = err << 1;
        if (e2 >= dy) {
            err += dy;
            x0 += sx;
        }
        if (e2 <= dx) {
            err += dx;
            y0 += sy;
        }
    }
}

void oxy_Vram16Rect(uint16_t x, uint8_t y, uint16_t width, uint16_t height)
{
    oxy_Vram16RectColor(x, y, width, height, oxy_vram16_color);
}

void oxy_Vram16RectColor(uint16_t x, uint8_t y, uint16_t width, uint16_t height, uint16_t color)
{
    uint16_t w = width;
    uint16_t h = height;

    if (!(oxy_vram16_shadow_enabled && oxy_vram16_shadow) && !oxy_Vram16Guard(true)) return;
    if (w == 0 || h == 0 || x >= OXY_VRAM16_WIDTH || y >= OXY_VRAM16_HEIGHT) return;
    if ((uint32_t)x + w > OXY_VRAM16_WIDTH) w = (uint16_t)(OXY_VRAM16_WIDTH - x);
    if ((uint32_t)y + h > OXY_VRAM16_HEIGHT) h = (uint16_t)(OXY_VRAM16_HEIGHT - y);

    if (oxy_vram16_shadow_enabled && oxy_vram16_shadow) {
        oxy_Vram16HLineColor(x, y, w, color);
        if (h > 1) oxy_Vram16HLineColor(x, (uint8_t)(y + h - 1), w, color);
        if (h > 2) {
            oxy_Vram16VLineColor(x, (uint8_t)(y + 1), (uint16_t)(h - 2), color);
            if (w > 1) oxy_Vram16VLineColor((uint16_t)(x + w - 1), (uint8_t)(y + 1), (uint16_t)(h - 2), color);
        }
    } else {
        oxy_Vram16Rectangle(x, y, w, h, color);
    }
}

void oxy_Vram16FillRect(uint16_t x, uint8_t y, uint16_t width, uint16_t height)
{
    oxy_Vram16FillRectColor(x, y, width, height, oxy_vram16_color);
}

void oxy_Vram16FillRectColor(uint16_t x, uint8_t y, uint16_t width, uint16_t height, uint16_t color)
{
    uint16_t w = width;
    uint16_t h = height;

    if (!(oxy_vram16_shadow_enabled && oxy_vram16_shadow) && !oxy_Vram16Guard(true)) return;
    if (w == 0 || h == 0 || x >= OXY_VRAM16_WIDTH || y >= OXY_VRAM16_HEIGHT) return;
    if ((uint32_t)x + w > OXY_VRAM16_WIDTH) w = (uint16_t)(OXY_VRAM16_WIDTH - x);
    if ((uint32_t)y + h > OXY_VRAM16_HEIGHT) h = (uint16_t)(OXY_VRAM16_HEIGHT - y);

    if (oxy_vram16_shadow_enabled && oxy_vram16_shadow) {
        for (uint16_t row = 0; row < h; row++) {
            oxy_Vram16HLineColor(x, (uint8_t)(y + row), w, color);
        }
    } else {
        oxy_Vram16FillRectangle(x, y, w, h, color);
    }
}

void oxy_Vram16Circle(int cx, int cy, int radius)
{
    oxy_Vram16CircleColor(cx, cy, radius, oxy_vram16_color);
}

void oxy_Vram16CircleColor(int cx, int cy, int radius, uint16_t color)
{
    int x = radius;
    int y = 0;
    int err = 1 - x;

    if (!(oxy_vram16_shadow_enabled && oxy_vram16_shadow) && !oxy_Vram16Guard(true)) return;
    if (radius < 0) return;

    while (x >= y) {
        int px[8] = {cx + x, cx + y, cx - y, cx - x, cx - x, cx - y, cx + y, cx + x};
        int py[8] = {cy + y, cy + x, cy + x, cy + y, cy - y, cy - x, cy - x, cy - y};

        for (int i = 0; i < 8; i++) {
            if (oxy_Vram16Inside(px[i], py[i])) {
                oxy_Vram16PutPixelTarget((uint16_t)px[i], (uint8_t)py[i], color);
            }
        }

        y++;
        if (err < 0) {
            err += 2 * y + 1;
        } else {
            x--;
            err += 2 * (y - x) + 1;
        }
    }
}

void oxy_Vram16ReplaceColor(uint16_t old_color, uint16_t new_color)
{
    if (old_color == new_color) {
        oxy_vram16_last_error = OXY_VRAM16_ERROR_NONE;
        return;
    }

    if (oxy_vram16_shadow_enabled && oxy_vram16_shadow) {
        oxy_Vram16ReplaceColorRect(0, 0, oxy_vram16_shadow_width, oxy_vram16_shadow_height,
                                   old_color, new_color);
        return;
    }

    if (!oxy_Vram16Guard(true)) return;
    vram16_ReplaceColor(old_color, new_color);
    oxy_vram16_last_error = OXY_VRAM16_ERROR_NONE;
}

void oxy_Vram16ReplaceColorRect(uint16_t x, uint8_t y, uint16_t width, uint8_t height,
                                uint16_t old_color, uint16_t new_color)
{
    uint16_t max_w;
    uint8_t max_h;
    uint16_t stride;
    uint16_t *base;

    if (width == 0 || height == 0 || old_color == new_color) {
        oxy_vram16_last_error = OXY_VRAM16_ERROR_NONE;
        return;
    }

    if (oxy_vram16_shadow_enabled && oxy_vram16_shadow) {
        if (x >= oxy_vram16_shadow_width || y >= oxy_vram16_shadow_height) {
            oxy_vram16_last_error = OXY_VRAM16_ERROR_NONE;
            return;
        }
        max_w = oxy_vram16_shadow_width;
        max_h = oxy_vram16_shadow_height;
        stride = oxy_vram16_shadow_stride;
        base = oxy_vram16_shadow;
    } else {
        if (!oxy_Vram16Guard(true)) return;
        if (x >= OXY_VRAM16_WIDTH || y >= OXY_VRAM16_HEIGHT) {
            oxy_vram16_last_error = OXY_VRAM16_ERROR_NONE;
            return;
        }
        max_w = OXY_VRAM16_WIDTH;
        max_h = OXY_VRAM16_HEIGHT;
        stride = OXY_VRAM16_WIDTH;
        base = (uint16_t *)0xD40000;
    }

    if ((uint32_t)x + width > max_w) {
        width = (uint16_t)(max_w - x);
    }
    if ((uint32_t)y + height > max_h) {
        height = (uint8_t)(max_h - y);
    }

    for (uint8_t row = 0; row < height; row++) {
        uint16_t *pixels = base + (uint16_t)(y + row) * stride + x;
        for (uint16_t col = 0; col < width; col++) {
            if (pixels[col] == old_color) {
                pixels[col] = new_color;
            }
        }
    }

    oxy_vram16_last_error = OXY_VRAM16_ERROR_NONE;
}

void oxy_Vram16FillCircle(int cx, int cy, int radius)
{
    oxy_Vram16FillCircleColor(cx, cy, radius, oxy_vram16_color);
}

void oxy_Vram16FillCircleColor(int cx, int cy, int radius, uint16_t color)
{
    int x = radius;
    int y = 0;
    int err = 1 - x;

    if (!(oxy_vram16_shadow_enabled && oxy_vram16_shadow) && !oxy_Vram16Guard(true)) return;
    if (radius < 0) return;

    while (x >= y) {
        oxy_Vram16LineColor(cx - x, cy + y, cx + x, cy + y, color);
        oxy_Vram16LineColor(cx - x, cy - y, cx + x, cy - y, color);
        oxy_Vram16LineColor(cx - y, cy + x, cx + y, cy + x, color);
        oxy_Vram16LineColor(cx - y, cy - x, cx + y, cy - x, color);

        y++;
        if (err < 0) {
            err += 2 * y + 1;
        } else {
            x--;
            err += 2 * (y - x) + 1;
        }
    }
}

void oxy_Vram16DrawImage(const oxy_vram16_image_t *image, int x, int y)
{
    if (!(oxy_vram16_shadow_enabled && oxy_vram16_shadow) && !oxy_Vram16Guard(true)) return;
    if (!image || !image->data || image->width == 0 || image->height == 0) return;

    for (uint16_t row = 0; row < image->height; row++) {
        int dy = y + row;
        if (dy < 0 || dy >= OXY_VRAM16_HEIGHT) continue;

        for (uint16_t col = 0; col < image->width; col++) {
            int dx = x + col;
            if (dx < 0 || dx >= OXY_VRAM16_WIDTH) continue;
            oxy_Vram16PutPixelTarget((uint16_t)dx, (uint8_t)dy, image->data[row * image->width + col]);
        }
    }
}

void oxy_Vram16DrawImageTransparent(const oxy_vram16_image_t *image, int x, int y, uint16_t transparent_color)
{
    if (!(oxy_vram16_shadow_enabled && oxy_vram16_shadow) && !oxy_Vram16Guard(true)) return;
    if (!image || !image->data || image->width == 0 || image->height == 0) return;

    for (uint16_t row = 0; row < image->height; row++) {
        int dy = y + row;
        if (dy < 0 || dy >= OXY_VRAM16_HEIGHT) continue;

        for (uint16_t col = 0; col < image->width; col++) {
            int dx = x + col;
            uint16_t px;
            if (dx < 0 || dx >= OXY_VRAM16_WIDTH) continue;
            px = image->data[row * image->width + col];
            if (px != transparent_color) {
                oxy_Vram16PutPixelTarget((uint16_t)dx, (uint8_t)dy, px);
            }
        }
    }
}

void oxy_Vram16DrawImageTransparentWithState(const oxy_vram16_image_t *image, int x, int y)
{
    if (oxy_vram16_transparent_enabled) {
        oxy_Vram16DrawImageTransparent(image, x, y, oxy_vram16_transparent);
        return;
    }
    oxy_Vram16DrawImage(image, x, y);
}

void oxy_Vram16DrawSubImage(const oxy_vram16_image_t *image,
                            int src_x, int src_y, int src_w, int src_h,
                            int dst_x, int dst_y)
{
    if (!(oxy_vram16_shadow_enabled && oxy_vram16_shadow) && !oxy_Vram16Guard(true)) return;
    if (!image || !image->data || src_w <= 0 || src_h <= 0) return;

    for (int row = 0; row < src_h; row++) {
        int sy = src_y + row;
        int dy = dst_y + row;
        if (sy < 0 || sy >= image->height || dy < 0 || dy >= OXY_VRAM16_HEIGHT) continue;

        for (int col = 0; col < src_w; col++) {
            int sx = src_x + col;
            int dx = dst_x + col;
            if (sx < 0 || sx >= image->width || dx < 0 || dx >= OXY_VRAM16_WIDTH) continue;
            oxy_Vram16PutPixelTarget((uint16_t)dx, (uint8_t)dy,
                                     image->data[sy * image->width + sx]);
        }
    }
}

void oxy_Vram16DrawImageScaledNearest(const oxy_vram16_image_t *image,
                                      int dst_x, int dst_y,
                                      int dst_w, int dst_h)
{
    if (!(oxy_vram16_shadow_enabled && oxy_vram16_shadow) && !oxy_Vram16Guard(true)) return;
    if (!image || !image->data || image->width == 0 || image->height == 0 ||
        dst_w <= 0 || dst_h <= 0) {
        return;
    }

    for (int y = 0; y < dst_h; y++) {
        int sy = (int)((uint32_t)y * image->height / (uint32_t)dst_h);
        int py = dst_y + y;
        if (py < 0 || py >= OXY_VRAM16_HEIGHT) continue;

        for (int x = 0; x < dst_w; x++) {
            int sx = (int)((uint32_t)x * image->width / (uint32_t)dst_w);
            int px = dst_x + x;
            if (px < 0 || px >= OXY_VRAM16_WIDTH) continue;
            oxy_Vram16PutPixelTarget((uint16_t)px, (uint8_t)py,
                                     image->data[sy * image->width + sx]);
        }
    }
}

void oxy_Vram16DrawImageScaledNearestWithState(const oxy_vram16_image_t *image,
                                               int dst_x, int dst_y,
                                               int dst_w, int dst_h)
{
    if (oxy_vram16_transparent_enabled) {
        oxy_Vram16DrawImageScaledNearestTransparent(image, dst_x, dst_y, dst_w, dst_h,
                                                    oxy_vram16_transparent);
        return;
    }
    oxy_Vram16DrawImageScaledNearest(image, dst_x, dst_y, dst_w, dst_h);
}

void oxy_Vram16DrawImageScaledNearestTransparent(const oxy_vram16_image_t *image,
                                                 int dst_x, int dst_y,
                                                 int dst_w, int dst_h,
                                                 uint16_t transparent_color)
{
    if (!(oxy_vram16_shadow_enabled && oxy_vram16_shadow) && !oxy_Vram16Guard(true)) return;
    if (!image || !image->data || image->width == 0 || image->height == 0 ||
        dst_w <= 0 || dst_h <= 0) {
        return;
    }

    for (int y = 0; y < dst_h; y++) {
        int sy = (int)((uint32_t)y * image->height / (uint32_t)dst_h);
        int py = dst_y + y;
        if (py < 0 || py >= OXY_VRAM16_HEIGHT) continue;

        for (int x = 0; x < dst_w; x++) {
            int sx = (int)((uint32_t)x * image->width / (uint32_t)dst_w);
            int px = dst_x + x;
            uint16_t sample;
            if (px < 0 || px >= OXY_VRAM16_WIDTH) continue;

            sample = image->data[sy * image->width + sx];
            if (sample != transparent_color) {
                oxy_Vram16PutPixelTarget((uint16_t)px, (uint8_t)py, sample);
            }
        }
    }
}

void oxy_Vram16DrawIndexedImage(const oxy_vram16_indexed_image_t *image, int x, int y)
{
    if (!(oxy_vram16_shadow_enabled && oxy_vram16_shadow) && !oxy_Vram16Guard(true)) return;
    if (!image || !image->data || image->width == 0 || image->height == 0) return;

    for (uint16_t row = 0; row < image->height; row++) {
        int dy = y + row;
        if (dy < 0 || dy >= OXY_VRAM16_HEIGHT) continue;

        for (uint16_t col = 0; col < image->width; col++) {
            int dx = x + col;
            uint8_t idx;
            if (dx < 0 || dx >= OXY_VRAM16_WIDTH) continue;

            idx = (uint8_t)(image->data[row * image->width + col] & 0x0F);
            oxy_Vram16PutPixelTarget((uint16_t)dx, (uint8_t)dy, oxy_vram16_palette16[idx]);
        }
    }
}

void oxy_Vram16DrawIndexedImageTransparent(const oxy_vram16_indexed_image_t *image,
                                           int x, int y,
                                           uint8_t transparent_index)
{
    if (!(oxy_vram16_shadow_enabled && oxy_vram16_shadow) && !oxy_Vram16Guard(true)) return;
    if (!image || !image->data || image->width == 0 || image->height == 0) return;

    transparent_index &= 0x0F;
    for (uint16_t row = 0; row < image->height; row++) {
        int dy = y + row;
        if (dy < 0 || dy >= OXY_VRAM16_HEIGHT) continue;

        for (uint16_t col = 0; col < image->width; col++) {
            int dx = x + col;
            uint8_t idx;
            if (dx < 0 || dx >= OXY_VRAM16_WIDTH) continue;

            idx = (uint8_t)(image->data[row * image->width + col] & 0x0F);
            if (idx != transparent_index) {
                oxy_Vram16PutPixelTarget((uint16_t)dx, (uint8_t)dy, oxy_vram16_palette16[idx]);
            }
        }
    }
}

void oxy_Vram16DrawIndexedImageScaledNearest(const oxy_vram16_indexed_image_t *image,
                                             int dst_x, int dst_y,
                                             int dst_w, int dst_h)
{
    if (!(oxy_vram16_shadow_enabled && oxy_vram16_shadow) && !oxy_Vram16Guard(true)) return;
    if (!image || !image->data || image->width == 0 || image->height == 0 ||
        dst_w <= 0 || dst_h <= 0) {
        return;
    }

    for (int y = 0; y < dst_h; y++) {
        int sy = (int)((uint32_t)y * image->height / (uint32_t)dst_h);
        int py = dst_y + y;
        if (py < 0 || py >= OXY_VRAM16_HEIGHT) continue;

        for (int x = 0; x < dst_w; x++) {
            int sx = (int)((uint32_t)x * image->width / (uint32_t)dst_w);
            int px = dst_x + x;
            uint8_t idx;
            if (px < 0 || px >= OXY_VRAM16_WIDTH) continue;

            idx = (uint8_t)(image->data[sy * image->width + sx] & 0x0F);
            oxy_Vram16PutPixelTarget((uint16_t)px, (uint8_t)py, oxy_vram16_palette16[idx]);
        }
    }
}

void oxy_Vram16DrawIndexedImageScaledNearestWithState(const oxy_vram16_indexed_image_t *image,
                                                      int dst_x, int dst_y,
                                                      int dst_w, int dst_h)
{
    if (oxy_vram16_transparent_enabled) {
        oxy_Vram16DrawIndexedImageScaledNearestTransparent(image, dst_x, dst_y, dst_w, dst_h,
                                                           oxy_vram16_transparent_index);
        return;
    }
    oxy_Vram16DrawIndexedImageScaledNearest(image, dst_x, dst_y, dst_w, dst_h);
}

void oxy_Vram16DrawIndexedImageScaledNearestTransparent(const oxy_vram16_indexed_image_t *image,
                                                        int dst_x, int dst_y,
                                                        int dst_w, int dst_h,
                                                        uint8_t transparent_index)
{
    if (!(oxy_vram16_shadow_enabled && oxy_vram16_shadow) && !oxy_Vram16Guard(true)) return;
    if (!image || !image->data || image->width == 0 || image->height == 0 ||
        dst_w <= 0 || dst_h <= 0) {
        return;
    }

    transparent_index &= 0x0F;
    for (int y = 0; y < dst_h; y++) {
        int sy = (int)((uint32_t)y * image->height / (uint32_t)dst_h);
        int py = dst_y + y;
        if (py < 0 || py >= OXY_VRAM16_HEIGHT) continue;

        for (int x = 0; x < dst_w; x++) {
            int sx = (int)((uint32_t)x * image->width / (uint32_t)dst_w);
            int px = dst_x + x;
            uint8_t idx;
            if (px < 0 || px >= OXY_VRAM16_WIDTH) continue;

            idx = (uint8_t)(image->data[sy * image->width + sx] & 0x0F);
            if (idx != transparent_index) {
                oxy_Vram16PutPixelTarget((uint16_t)px, (uint8_t)py, oxy_vram16_palette16[idx]);
            }
        }
    }
}

void oxy_Vram16SetFont(const oxy_vram16_font_t *font)
{
    oxy_vram16_font = font;
}

const oxy_vram16_font_t* oxy_Vram16GetFont(void)
{
    return oxy_Vram16ResolveFont();
}

void oxy_Vram16SetCursor(uint16_t x, uint8_t y)
{
    oxy_vram16_cursor_x = x;
    oxy_vram16_cursor_y = y;
}

uint16_t oxy_Vram16GetCursorX(void)
{
    return oxy_vram16_cursor_x;
}

uint8_t oxy_Vram16GetCursorY(void)
{
    return oxy_vram16_cursor_y;
}

void oxy_Vram16SetTextScale(uint8_t x_scale, uint8_t y_scale)
{
    oxy_vram16_scale_x = x_scale ? x_scale : 1;
    oxy_vram16_scale_y = y_scale ? y_scale : 1;
}

void oxy_Vram16SetTextSize(uint8_t size)
{
    uint8_t clamped = size ? size : 1;
    oxy_vram16_scale_x = clamped;
    oxy_vram16_scale_y = clamped;
}

uint8_t oxy_Vram16GetTextSize(void)
{
    return oxy_vram16_scale_x;
}

uint8_t oxy_Vram16GetTextScaleX(void)
{
    return oxy_vram16_scale_x;
}

uint8_t oxy_Vram16GetTextScaleY(void)
{
    return oxy_vram16_scale_y;
}

void oxy_Vram16DrawChar(int x, int y, char c)
{
    oxy_Vram16DrawCharColor(x, y, c, oxy_vram16_color, oxy_vram16_back_color, false);
}

void oxy_Vram16DrawCharColor(int x, int y, char c, uint16_t fg, uint16_t bg, bool draw_bg)
{
    if (!(oxy_vram16_shadow_enabled && oxy_vram16_shadow) && !oxy_Vram16Guard(true)) return;
    oxy_Vram16DrawGlyphBitmap(x, y, (unsigned char)c, fg, bg, draw_bg);
}

void oxy_Vram16DrawString(int x, int y, const char *text)
{
    oxy_Vram16DrawStringColor(x, y, text, oxy_vram16_color, oxy_vram16_back_color, false);
}

void oxy_Vram16DrawStringColor(int x, int y, const char *text, uint16_t fg, uint16_t bg, bool draw_bg)
{
    int pen_x = x;
    int pen_y = y;
    const oxy_vram16_font_t *font = oxy_Vram16ResolveFont();
    int glyph_w = font->glyph_width;
    int glyph_h = font->glyph_height;

    if (!(oxy_vram16_shadow_enabled && oxy_vram16_shadow) && !oxy_Vram16Guard(true)) return;
    if (!text) return;

    for (size_t i = 0; text[i] != '\0'; i++) {
        char c = text[i];

        if (c == '\n') {
            pen_x = x;
            pen_y += (glyph_h + 1) * oxy_vram16_scale_y;
            continue;
        }

        oxy_Vram16DrawCharColor(pen_x, pen_y, c, fg, bg, draw_bg);
        pen_x += (glyph_w + 1) * oxy_vram16_scale_x;
    }
}

void oxy_Vram16Print(const char *text)
{
    const oxy_vram16_font_t *font = oxy_Vram16ResolveFont();
    int glyph_w = font->glyph_width;
    int glyph_h = font->glyph_height;

    if (!(oxy_vram16_shadow_enabled && oxy_vram16_shadow) && !oxy_Vram16Guard(true)) return;
    if (!text) return;

    for (size_t i = 0; text[i] != '\0'; i++) {
        char c = text[i];

        if (c == '\n') {
            oxy_vram16_cursor_x = 0;
            oxy_vram16_cursor_y = (uint8_t)(oxy_vram16_cursor_y + (glyph_h + 1) * oxy_vram16_scale_y);
            continue;
        }

        oxy_Vram16DrawChar((int)oxy_vram16_cursor_x, (int)oxy_vram16_cursor_y, c);
        oxy_vram16_cursor_x = (uint16_t)(oxy_vram16_cursor_x + (glyph_w + 1) * oxy_vram16_scale_x);

        if (oxy_vram16_cursor_x + glyph_w >= OXY_VRAM16_WIDTH) {
            oxy_vram16_cursor_x = 0;
            oxy_vram16_cursor_y = (uint8_t)(oxy_vram16_cursor_y + (glyph_h + 1) * oxy_vram16_scale_y);
        }
    }
}
