#include "entry.h"

#include "../util.h"
#include "../loop.h"
#include "../color.h"
#include "../../oxy_cursor.h"
#include "../../oxy_gfx.h"

#include <graphx.h>
#include <keypadc.h>
#include <string.h>

static char oxy_EntryCharacter(enum entry_mode_t mode, uint8_t scan_code)
{
    static const char upper[] = "\0\0\0\0\0\0\0\0\0\0\0WRMH\0\0?\0VQLG\0\0.ZUPKFC\0 YTOJEB\0\0XSNIDA";
    static const char lower[] = "\0\0\0\0\0\0\0\0\0\0\0wrmh\0\0?\0vqlg\0\0.zupkfc\0 ytojeb\0\0xsnida";
    static const char number[] = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0-\x33\x36\x39)\0\0\0.\x32\x35\x38(\0\0\0\x30\x31\x34\x37,";
    const char *map = mode == OXY_ENTRY_MODE_LOWER ? lower : mode == OXY_ENTRY_MODE_NUM ? number : upper;

    return scan_code < sizeof(upper) ? map[scan_code] : '\0';
}

void oxy_UpdateEntry(struct oxy_widget_t *widget)
{
    struct oxy_entry_t *entry = (struct oxy_entry_t *)widget;
    const struct oxy_input_event_t *event = oxy_GetInputEvent();

    if (!widget->state.visible || !entry->text || !entry->max_chars)
        return;

    if (event->primary_pressed && oxy_CheckCursorOverlap(widget))
    {
        int pointer_x = widget->position.x + 3;
        size_t index = 0;
        oxy_FocusWidget(widget);
        while (entry->text[index] && pointer_x + (int)gfx_GetCharWidth(entry->password ? '*' : entry->text[index]) / 2 < event->pointer_x)
        {
            pointer_x += gfx_GetCharWidth(entry->password ? '*' : entry->text[index]);
            index++;
        }
        entry->cursor_pos = index;
    }

    widget->state.selected = oxy_IsWidgetFocused(widget);
    if (!oxy_IsWidgetFocused(widget))
        return;

    uint8_t scan_code = event->scan_code;
    size_t length = strlen(entry->text);
    if (entry->cursor_pos > length)
        entry->cursor_pos = length;

    if (scan_code == sk_Alpha)
        entry->mode = (entry->mode + 1) % 3;
    else if (scan_code == sk_Left && entry->cursor_pos > 0)
        entry->cursor_pos--;
    else if (scan_code == sk_Right && entry->cursor_pos < length)
        entry->cursor_pos++;
    else if (scan_code == sk_Del && entry->cursor_pos > 0)
    {
        memmove(entry->text + entry->cursor_pos - 1,
                entry->text + entry->cursor_pos,
                length - entry->cursor_pos + 1);
        entry->cursor_pos--;
    }
    else
    {
        char character = oxy_EntryCharacter(entry->mode, scan_code);
        if (character && length < entry->max_chars)
        {
            memmove(entry->text + entry->cursor_pos + 1,
                    entry->text + entry->cursor_pos,
                    length - entry->cursor_pos + 1);
            entry->text[entry->cursor_pos++] = character;
        }
    }

    if (scan_code)
        widget->state.redraw = true;
}

void oxy_RenderEntry(struct oxy_widget_t *widget)
{
    struct oxy_entry_t *entry = (struct oxy_entry_t *)widget;

    if (!widget->state.visible)
        return;

    oxy_OutlinedRectangle(widget->position.x, widget->position.y,
                          widget->size.width, widget->size.height,
                          widget->state.selected ? widget->color.selected : widget->color.unselected,
                          widget->color.color_a);
    oxy_SetTextColor(widget->state.selected ? widget->color.text_fg_selected : widget->color.text_fg_unselected,
                     widget->state.selected ? widget->color.text_bg_selected : widget->color.text_bg_unselected);

    int text_x = widget->position.x + 3;
    int text_y = widget->position.y + (widget->size.height - 8) / 2;
    int cursor_x = text_x;
    if (entry->text)
    {
        gfx_SetTextXY(text_x, text_y);
        for (size_t i = 0; entry->text[i]; i++)
        {
            char character = entry->password ? '*' : entry->text[i];
            if (i < entry->cursor_pos)
                cursor_x += gfx_GetCharWidth(character);
            gfx_PrintChar(character);
        }
    }

    if (widget->state.selected)
    {
        gfx_SetColor(widget->color.text_fg_selected);
        gfx_VertLine(cursor_x, text_y, 8);
    }

    widget->state.redraw = false;
}