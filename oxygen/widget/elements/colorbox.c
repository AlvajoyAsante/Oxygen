#include "colorbox.h"

#include "../util.h"
#include "../loop.h"
#include "../color.h"
#include "../../oxy_cursor.h"
#include "../../oxy_gfx.h"

#include <graphx.h>
#include <keypadc.h>

void oxy_UpdateColorbox(struct oxy_widget_t *widget)
{
    if (!widget->state.visible)
        return;

    if (CURSOR_STATE_ID == CURSOR_STATE_IDLE && oxy_CheckCursorOverlap(widget))
    {
        if (widget->state.clickable)
        {
            widget->state.selected = true;
            CURSOR_STATE_ID = CURSOR_STATE_CLICK;
        }
        else
        {
            widget->state.selected = false;
        }
    }
    else
    {
        widget->state.selected = false;
    }

    if (kb_Data[6] & kb_Enter || kb_Data[1] & kb_2nd)
    {
        if (!widget->state.clicked && widget->state.selected)
        {
            widget->state.clicked = true;
            // Pull oxygen color input here.
        }
    }
    else
    {
        widget->state.clicked = false;
    }
}

void oxy_RenderColorbox(struct oxy_widget_t *widget)
{
    struct oxy_colorbox_t *colorbox = (struct oxy_colorbox_t *)widget;

    if (widget->state.visible)
    {
        if (widget->state.selected)
        {
            if (widget->state.clicked)
            {
                oxy_OutlinedRectangle(widget->position.x, widget->position.y, widget->size.width, widget->size.height,
                                      widget->color.clicked, widget->color.color_b);
            }
            else
            {
                oxy_OutlinedRectangle(widget->position.x, widget->position.y, widget->size.width, widget->size.height,
                                      widget->color.selected, widget->color.color_b);
            }
        }
        else
        {
            oxy_OutlinedRectangle(widget->position.x, widget->position.y, widget->size.width, widget->size.height,
                                  widget->color.unselected, widget->color.color_b);
        }

        gfx_SetColor(colorbox->index);
        gfx_FillRectangle(widget->position.x + 2, widget->position.y + 2, widget->size.width - 4, widget->size.height - 4);
    }
}