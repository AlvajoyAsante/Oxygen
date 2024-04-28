#include "scrollbar.h"
#include <tice.h>
#include <graphx.h>

void oxy_UpdateScrollbar(struct oxy_widget_t *widget)
{
    struct oxy_scrollbar_t *scrollbar = (struct oxy_scrollbar_t *)widget;

    /* Handle Redraw */
}

void oxy_RenderScrollbar(struct oxy_widget_t *widget)
{
    struct oxy_scrollbar_t *scrollbar = (struct oxy_scrollbar_t *)widget;

    /* Handle Redrawing */
    if (widget->state.redraw)
    {
        /* Render Options */
        
    }
}