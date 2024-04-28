#include "hmargin.h"

#include "../color.h"

#include <graphx.h>

void oxy_RenderHmargin(struct oxy_widget_t *widget)
{
	struct oxy_hmargin_t *hmargin = (struct oxy_hmargin_t *)widget;

	gfx_SetColor(widget->color.color_a);

	gfx_HorizLine(widget->position.x,
				  widget->position.y - 1 + (int)(hmargin->alignment == OXY_ALIGN_BOTTOM) * (widget->size.width + 1),
				  widget->size.width);
}