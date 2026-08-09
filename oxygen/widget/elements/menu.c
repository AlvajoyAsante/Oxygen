#include "menu.h"

#include <graphx.h>
#include <keypadc.h>

#include "label.h"
#include "sprite.h"
#include "button.h"

#include "../util.h"
#include "../loop.h"
#include "../color.h"
#include "../../oxy_cursor.h"
#include "../../oxy_gfx.h"

void oxy_AlignMenu(struct oxy_menu_t *menu)
{
    if (!menu || !menu->widget.child || !menu->rows || !menu->columns)
        return;

    int count = menu->rows * menu->columns;
    for (int i = 0; menu->widget.child[i]; i++)
    {
        struct oxy_widget_t *child = menu->widget.child[i];

        if (i >= count)
        {
            child->state.visible = false;
            continue;
        }

        int column = i % menu->columns;
        int row = i / menu->columns;
        int cell_width = menu->widget.size.width / menu->columns;
        int cell_height = menu->widget.size.height / menu->rows;

        child->state.visible = true;
        child->position.x = menu->widget.position.x + column * cell_width;
        child->position.y = menu->widget.position.y + row * cell_height;
        child->size.width = cell_width + (column == menu->columns - 1 ? menu->widget.size.width % menu->columns : 0);
        child->size.height = cell_height + (row == menu->rows - 1 ? menu->widget.size.height % menu->rows : 0);

        if (child->child)
            oxy_AlignChildren(child);
    }
}

void oxy_UpdateMenu(struct oxy_widget_t *widget)
{
    struct oxy_menu_t *menu = (struct oxy_menu_t *)widget;
    const struct oxy_input_event_t *event = oxy_GetInputEvent();

    if (!widget->state.visible || !menu->rows || !menu->columns)
        return;

    widget->state.selected = oxy_CheckCursorOverlap(widget) || oxy_IsWidgetFocused(widget);
    if (event->primary_pressed && oxy_CheckCursorOverlap(widget))
    {
        oxy_FocusWidget(widget);
        oxy_CaptureWidget(widget);
    }
    if (widget->state.selected)
    {
        int column = (CURSOR_X_POS - widget->position.x) * menu->columns / widget->size.width;
        int row = (CURSOR_Y_POS - widget->position.y) * menu->rows / widget->size.height;
        int selection = row * menu->columns + column;

        if (selection >= 0 && selection < menu->rows * menu->columns && selection != menu->selection)
        {
            menu->selection = selection;
            widget->state.redraw = true;
        }
    }

    if (oxy_IsWidgetFocused(widget))
    {
        int selection = menu->selection;
        if (event->scan_code == sk_Left && selection % menu->columns > 0) selection--;
        if (event->scan_code == sk_Right && selection % menu->columns + 1 < menu->columns) selection++;
        if (event->scan_code == sk_Up && selection >= menu->columns) selection -= menu->columns;
        if (event->scan_code == sk_Down && selection + menu->columns < menu->rows * menu->columns)
            selection += menu->columns;
        if (selection != menu->selection)
        {
            menu->selection = selection;
            widget->state.redraw = true;
        }
    }

    widget->state.clicked = event->primary_down && event->captured == widget;

    if (widget->child)
    {
        oxy_AlignMenu(menu);
        oxy_UpdateStack(widget->child);
    }
}

void oxy_RenderMenu(struct oxy_widget_t *widget)
{
    struct oxy_menu_t *menu = (struct oxy_menu_t *)widget;

    if (!widget->state.visible || !menu->rows || !menu->columns)
        return;

    int cell_width = widget->size.width / menu->columns;
    int cell_height = widget->size.height / menu->rows;
    for (int option = 0; option < menu->rows * menu->columns; option++)
    {
        int column = option % menu->columns;
        int row = option / menu->columns;
        int x = widget->position.x + column * cell_width;
        int y = widget->position.y + row * cell_height;
        int width = cell_width + (column == menu->columns - 1 ? widget->size.width % menu->columns : 0);
        int height = cell_height + (row == menu->rows - 1 ? widget->size.height % menu->rows : 0);
        bool selected = option == menu->selection;

        oxy_OutlinedRectangle(x, y, width, height,
                              selected ? widget->color.selected : widget->color.unselected,
                              widget->color.color_a);

        if (menu->sprite && menu->sprite[option])
        {
            gfx_sprite_t *sprite = menu->sprite[option];
            gfx_TransparentSprite(sprite, x + (width - sprite->width) / 2, y + (height - sprite->height) / 2);
        }

        if (menu->text && menu->text[option])
        {
            char *text = menu->text[option];
            oxy_SetTextColor(selected ? widget->color.text_fg_selected : widget->color.text_fg_unselected,
                             selected ? widget->color.text_bg_selected : widget->color.text_bg_unselected);
            gfx_PrintStringXY(text, x + (width - gfx_GetStringWidth(text)) / 2, y + (height - 8) / 2);
        }
    }

    if (widget->child)
        oxy_RenderStack(widget->child);

    widget->state.redraw = false;
}