#include "sprite.h"

#include <tice.h>
#include <graphx.h>

void oxy_RenderSprite(struct oxy_widget_t *widget)
{
    struct oxy_sprite_t *sprite = (struct oxy_sprite_t *)widget;

    if (widget->state.visible)
    {
        if (sprite->transparent)
        {
            gfx_SetTransparentColor(sprite->transparent_color_index);
            gfx_ScaledTransparentSprite_NoClip(sprite->spr, widget->position.x, widget->position.y, sprite->x_scale, sprite->y_scale);
        }
        else
        {
            gfx_ScaledSprite_NoClip(sprite->spr, widget->position.x, widget->position.y, sprite->x_scale, sprite->y_scale);
        }
    }
}
