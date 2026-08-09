#include "loop.h"
#include "util.h"
#include "../oxy_cursor.h"
#include "elements/window.h"

#include <tice.h>
#include <keypadc.h>

static struct oxy_config_t oxy_config_storage;
struct oxy_config_t *oxy_config = &oxy_config_storage;
static struct oxy_input_event_t oxy_input;
static bool oxy_primary_was_down;
static bool oxy_secondary_was_down;

static bool oxy_PointInsideWidget(const struct oxy_widget_t *widget, int x, int y)
{
	return widget && widget->state.visible && widget->state.clickable &&
		x >= widget->position.x && x < widget->position.x + widget->size.width &&
		y >= widget->position.y && y < widget->position.y + widget->size.height;
}

static struct oxy_widget_t *oxy_HitTestStack(struct oxy_widget_t **stack, int x, int y)
{
	int length;
	if (!stack) return NULL;
	length = oxy_GetStackLength(stack);
	while (length-- > 0)
	{
		struct oxy_widget_t *widget = stack[length];
		struct oxy_widget_t *child;
		if (!widget || !widget->state.visible) continue;
		child = oxy_HitTestStack(widget->child, x, y);
		if (child) return child;
		if (oxy_PointInsideWidget(widget, x, y)) return widget;
	}
	return NULL;
}

const struct oxy_input_event_t *oxy_GetInputEvent(void)
{
	return &oxy_input;
}

void oxy_FocusWidget(struct oxy_widget_t *widget)
{
	if (widget && (!widget->state.visible || !widget->state.clickable)) return;
	oxy_input.focused = widget;
}

bool oxy_IsWidgetFocused(const struct oxy_widget_t *widget)
{
	return widget && oxy_input.focused == widget;
}

bool oxy_CaptureWidget(struct oxy_widget_t *widget)
{
	if (!widget || (oxy_input.captured && oxy_input.captured != widget)) return false;
	oxy_input.captured = widget;
	return true;
}

void oxy_ReleaseWidgetCapture(struct oxy_widget_t *widget)
{
	if (!widget || oxy_input.captured == widget) oxy_input.captured = NULL;
}

static void oxy_UpdateInputEvent(void)
{
	bool primary_down = (kb_Data[6] & kb_Enter) || (kb_Data[1] & kb_2nd);
	bool secondary_down = kb_Data[2] & kb_Alpha;
	oxy_input.pointer_x = CURSOR_X_POS;
	oxy_input.pointer_y = CURSOR_Y_POS;
	oxy_input.scan_code = os_GetCSC();
	oxy_input.primary_down = primary_down;
	oxy_input.primary_pressed = primary_down && !oxy_primary_was_down;
	oxy_input.primary_released = !primary_down && oxy_primary_was_down;
	oxy_input.secondary_down = secondary_down;
	oxy_input.secondary_pressed = secondary_down && !oxy_secondary_was_down;
	oxy_input.secondary_released = !secondary_down && oxy_secondary_was_down;
	oxy_input.hovered = oxy_HitTestStack(oxy_config->stack, CURSOR_X_POS, CURSOR_Y_POS);
	if (oxy_input.primary_pressed)
	{
		oxy_FocusWidget(oxy_input.hovered);
		if (oxy_input.hovered) oxy_CaptureWidget(oxy_input.hovered);
	}
	oxy_primary_was_down = primary_down;
	oxy_secondary_was_down = secondary_down;
}

void oxy_UpdateStack(struct oxy_widget_t **stack)
{
	if (!stack)
		return;

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
	if (!oxy_config->stack)
		return;

	oxy_UpdateCursor();
	oxy_UpdateInputEvent();
	oxy_UpdateWindowManager((kb_Data[6] & kb_Enter) || (kb_Data[1] & kb_2nd));
	oxy_UpdateStack(oxy_config->stack);
	if (oxy_input.primary_released) oxy_ReleaseWidgetCapture(NULL);
}

// ---

void oxy_RenderStack(struct oxy_widget_t **stack)
{
	if (!stack)
		return;

	for (struct oxy_widget_t **p = stack; *p; ++p)
	{
		struct oxy_widget_t *w = *p;
		if (w->state.visible && w->render)
		{
			w->render(w);
		}
	}
}

void oxy_RenderWidgets(void)
{
	if (!oxy_config->stack)
		return;

	oxy_RenderWindowBackground();
	oxy_RenderStack(oxy_config->stack);
	oxy_RenderCursor();
}