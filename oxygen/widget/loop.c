#include "loop.h"
#include "../oxy_cursor.h"
#include "elements/window.h"

#include <tice.h>
#include <keypadc.h>

struct oxy_config_t *oxy_config;

void oxy_UpdateStack(struct oxy_widget_t **stack)
{
	for (struct oxy_widget_t **p = stack; *p; ++p)
	{
		struct oxy_widget_t *w = *p;

		if (!w->state.visible)
			continue;

		if (w->update)
		{
			w->update(w);
		}

		if (w->state.clicked)
		{
			for (struct oxy_widget_t **q = stack; q < p; ++q)
			{
				(*q)->state.clicked = false;
			}
		}
	}
}

void oxy_UpdateWidgets(void)
{
	oxy_UpdateCursor();
	oxy_UpdateStack(oxy_config->stack);
}

// ---

void oxy_RenderStack(struct oxy_widget_t **stack)
{
	for (struct oxy_widget_t **p = stack; *p; ++p)
	{
		struct oxy_widget_t *w = *p;
		if (w->render)
		{
			w->render(w);
		}
	}
}

void oxy_RenderWidgets(void)
{
	oxy_RenderStack(oxy_config->stack);
	oxy_RenderCursor();
}