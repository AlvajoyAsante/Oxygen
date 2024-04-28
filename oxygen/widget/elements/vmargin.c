#include "vmargin.h"

#include "../color.h"

#include <graphx.h>

void oxy_RenderVmargin(struct oxy_widget_t *widget)
{
	struct oxy_vmargin_t *vmargin = (struct oxy_vmargin_t *)widget;

	gfx_SetColor(widget->color.color_a);
	
	gfx_VertLine(widget->position.x - 1 + (vmargin->alignment == VMARGIN_ALIGN_RIGHT) * (widget->size.width + 1),
				 widget->position.y,
				 widget->size.height);
}