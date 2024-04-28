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
    int i = 0;

    while (menu->widget.child[i])
    {
        struct oxy_widget_t *child = menu->widget.child[i];

        if (i > (int)((menu->rows) * ((menu->columns) - 1)))
        {
            child->state.visible = false;
            i++;
            continue;
        }
        else
        {
            child->state.visible = true;
        }

        if (child->type == OXY_BUTTON_TYPE)
        {

            int default_width = menu->widget.size.width / menu->columns;
            int default_height = menu->widget.size.height / menu->rows;

            // resize the last entry in the row if it isn't the right size, because it irritates me if it's not
            child->size.width = default_width + (i % (menu->columns - 1) == 0) * (menu->widget.size.width % menu->columns);

            // the last row, i / menu->rows will be equal to 1
            child->size.height = default_height + (i / (menu->rows - 1)) * (menu->widget.size.height % menu->rows);
            child->position.x = menu->widget.position.x + i % menu->columns * default_width;
            child->position.y = menu->widget.position.y + i / menu->columns * default_height;

            // realign it if necessary
            if (child->child)
            {
                int j = 0;
                while (child->child[j])
                {
                    if (child->child[j]->type == OXY_LABEL_TYPE)
                    {
                        struct oxy_label_t *child_text = (struct oxy_label_t *)child->child[j];
                        oxy_AlignWidgetToWidget(child->child[j], child, child_text->widget.position.x_align, child_text->widget.position.y_align);
                    }
                    else if (child->type == OXY_SPRITE_TYPE)
                    {
                        struct oxy_sprite_t *child_sprite = (struct oxy_sprite_t *)child->child[j];
                        oxy_AlignWidgetToWidget(child->child[j], child, child_sprite->widget.position.x_align, child_sprite->widget.position.y_align);
                    }
                    j++;
                }
            }
        }

        i++;
    }
}

void oxy_UpdateMenu(struct oxy_widget_t *widget)
{
    struct oxy_menu_t *menu = (struct oxy_menu_t *)widget;

    bool found_selection = false;
    int i = 0;

    // check if it overlaps with the cursor
    if (oxy_CheckCursorOverlap(widget))
    {
        widget->state.selected = true;
    }
    else
    {
        widget->state.selected = false;
    }

    // see which option is selected
    while (widget->child[i] != NULL)
    {
        // we're hoping this is a button
        if (widget->state.selected)
        {
            if (found_selection)
            {
                widget->child[i]->state.selected = false;
            }
            else
            {
                widget->child[i]->update(widget->child[i]);
                if (widget->child[i]->type == OXY_BUTTON_TYPE)
                {
                    // check if it's selected now
                    if (widget->child[i]->state.selected)
                    {
                        menu->selection = i;
                        // we can only have one selected item
                        found_selection = true;
                    }
                }
            }
        }
        else
        {
            widget->child[i]->state.selected = false;
        }
        i++;
    }
}

static void oxy_RenderOption(int option, struct oxy_menu_t *menu, char *option_text, gfx_sprite_t *option_spr)
{
    struct oxy_widget_t *widget = (struct oxy_widget_t *)menu;

    // If the widget doesn't need to be redrawn, or the option is not selected, then don't render it.
    if (!widget->state.redraw && option != menu->selection)
        return;

    // Create a button widget to represent the menu option.
    struct oxy_button_t button;
    oxy_InitializeWidget(&button.widget, OXY_BUTTON_TYPE);

    button.widget.state.clicked = (option == menu->selection);

    // button.widget.color.clicked = menu->widget.color.color_a

    // If the option has text, then add a text widget to the button widget.
    if (option_text)
    {
        struct oxy_label_t label;
        memcpy(&label.widget, &(menu->text), sizeof(struct oxy_label_t));
        label.text = option_text;
        label.widget.child = NULL;

        button.widget.child[0] = &label.widget;
    }

    // If the option has a sprite, then add a sprite widget to the button widget.
    if (option_spr)
    {
        struct oxy_sprite_t sprite;
        memcpy(&sprite.widget, &(menu->sprite), sizeof(struct oxy_sprite_t));
        sprite.spr = option_spr;
        sprite.widget.child = NULL;
        button.widget.child[(option_text != NULL)] = &sprite.widget;
    }

    // If the menu has a child widget at this option, then add it to the button widget.
    if (widget->child && widget->child[option])
    {
        button.widget.child[(option_text != NULL) + (option_spr != NULL)] = widget->child[option];
    }

    // Set the button widget's null pointer at the end of the child array.
    button.widget.child[(option_text != NULL) + (option_spr != NULL) + (widget->child != NULL && widget->child[option] != NULL)] = NULL;

    // Set the button widget's transform.
    oxy_SetMenuOptionTransform(option, &button, menu);

    // Align the button widget's child widgets.
    oxy_RecursiveAlign(&button.widget);

    // Set the button widget's selected state to true if it is selected and the selection box is not hidden.
    button.widget.state.selected = (option == menu->selection);

    // Set the button widget's needs redraw state to true.
    oxy_RecursiveSetNeedsRedraw(button.widget.child);

    // Render the button widget.
    button.widget.render(&button.widget);
}

void oxy_RenderMenu(struct oxy_widget_t *widget)
{
    struct oxy_menu_t *menu = (struct oxy_menu_t *)widget;

    for (int i = 0; i < menu->rows * menu->columns; i++)
    {
        oxy_RenderOption(i, menu, menu->text && menu->text[i] ? menu->text[i] : NULL,
                         menu->sprite && menu->sprite[i] ? menu->sprite[i] : NULL);
    }
}