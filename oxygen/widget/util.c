#include "util.h"
#include "color.h"
#include "../oxy_cursor.h"

#include "elements/button.h"
#include "elements/checkbox.h"
#include "elements/colorbox.h"
#include "elements/dropdown.h"
#include "elements/hmargin.h"
#include "elements/vmargin.h"
#include "elements/menu.h"
#include "elements/rectangle.h"
#include "elements/scrollbar.h"
#include "elements/slider.h"
#include "elements/sprite.h"
#include "elements/switch.h"
#include "elements/label.h"
#include "elements/entry.h"
#include "elements/window.h"

void oxy_SetWidgetDescription(struct oxy_widget_t *widget, char *text)
{
	widget->cursor_info.description = text;
}

void oxy_SetWidgetRightClick(struct oxy_widget_t *widget, void *function, void *arg)
{

	widget->cursor_info.right_click = function;
	widget->cursor_info.right_arg = arg;
}

void oxy_SeWidgetLeftClick(struct oxy_widget_t *widget, void *function, void *arg)
{
	widget->cursor_info.left_click = function;
	widget->cursor_info.left_arg = arg;
}

void oxy_SetWidgetHover(struct oxy_widget_t *widget, void *function, void *arg)
{
	widget->cursor_info.hover = function;
	widget->cursor_info.hover_arg = arg;
}

// --------

static bool oxy_DoesTypeExist(uint8_t type)
{
	switch (type)
	{
	case OXY_BUTTON_TYPE:
		return 1;
	case OXY_CHECKBOX_TYPE:
		return 1;
	case OXY_COLORBOX_TYPE:
		return 1;
	case OXY_DROPDOWN_TYPE:
		return 1;
	case OXY_HMARGIN_TYPE:
		return 1;
	case OXY_VMARGIN_TYPE:
		return 1;
	case OXY_MENU_TYPE:
		return 1;
	case OXY_RECTANGLE_TYPE:
		return 1;
	case OXY_SCROLLBAR_TYPE:
		return 1;
	case OXY_SLIDER_TYPE:
		return 1;
	case OXY_SPRITE_TYPE:
		return 1;
	case OXY_SWITCH_TYPE:
		return 1;
	case OXY_LABEL_TYPE:
		return 1;
	case OXY_ENTRY_TYPE:
		return 1;
	case OXY_WINDOW_TYPE:
		return 1;

	default:
		return 0;
	}
}

static void *oxy_ReturnUpdateByType(uint8_t type)
{
	switch (type)
	{
	case OXY_BUTTON_TYPE:
		return oxy_UpdateButton;

	case OXY_CHECKBOX_TYPE:
		return oxy_UpdateCheckbox;

	case OXY_COLORBOX_TYPE:
		return oxy_UpdateColorbox;

	case OXY_DROPDOWN_TYPE:
		return oxy_UpdateDropDown;

	case OXY_MENU_TYPE:
		return oxy_UpdateMenu;

	case OXY_SCROLLBAR_TYPE:
		return oxy_UpdateScrollbar;

	case OXY_SLIDER_TYPE:
		return oxy_UpdateSlider;

	case OXY_SWITCH_TYPE:
		return oxy_UpdateSwitch;

	case OXY_ENTRY_TYPE:
		return oxy_UpdateEntry;

	case OXY_WINDOW_TYPE:
		return oxy_UpdateWindow;

	default:
		return NULL;
	}
}

static void *oxy_ReturnRenderByType(uint8_t type)
{
	switch (type)
	{
	case OXY_BUTTON_TYPE:
		return oxy_RenderButton;

	case OXY_CHECKBOX_TYPE:
		return oxy_RenderCheckbox;

	case OXY_COLORBOX_TYPE:
		return oxy_RenderColorbox;

	case OXY_DROPDOWN_TYPE:
		return oxy_RenderDropDown;

	case OXY_HMARGIN_TYPE:
		return oxy_RenderHmargin;

	case OXY_VMARGIN_TYPE:
		return oxy_RenderVmargin;

	case OXY_MENU_TYPE:
		return oxy_RenderMenu;

	case OXY_RECTANGLE_TYPE:
		return oxy_RenderRectangle;

	case OXY_SCROLLBAR_TYPE:
		return oxy_RenderScrollbar;

	case OXY_SLIDER_TYPE:
		return oxy_RenderSlider;

	case OXY_SPRITE_TYPE:
		return oxy_RenderSprite;

	case OXY_SWITCH_TYPE:
		return oxy_RenderSwitch;

	case OXY_LABEL_TYPE:
		return oxy_RenderLabel;

	case OXY_ENTRY_TYPE:
		return oxy_RenderEntry;

	case OXY_WINDOW_TYPE:
		return oxy_RenderWindow;

	default:
		return NULL;
	}
}

static bool oxy_ReturnClickableByType(uint8_t type)
{
	switch (type)
	{
	case OXY_BUTTON_TYPE:
		return 1;

	case OXY_CHECKBOX_TYPE:
		return 1;

	case OXY_COLORBOX_TYPE:
		return 1;

	case OXY_DROPDOWN_TYPE:
		return 1;

	case OXY_HMARGIN_TYPE:
		return 0;

	case OXY_VMARGIN_TYPE:
		return 0;

	case OXY_MENU_TYPE:
		return 1;

	case OXY_RECTANGLE_TYPE:
		return 0;

	case OXY_SCROLLBAR_TYPE:
		return 1;

	case OXY_SLIDER_TYPE:
		return 1;

	case OXY_SPRITE_TYPE:
		return 0;

	case OXY_SWITCH_TYPE:
		return 1;

	case OXY_LABEL_TYPE:
		return 0;

	case OXY_ENTRY_TYPE:
		return 1;

	case OXY_WINDOW_TYPE:
		return 1;

	default:
		return NULL;
	}
}

bool oxy_InitWidget(struct oxy_widget_t *widget, uint8_t type)
{
	// Just in case type does not exist (type not found)
	if (!widget || !oxy_DoesTypeExist(type))
		return 0;

	// Set simple widget information
	widget->child = NULL;
	widget->type = type;
	widget->state.selected = false;
	widget->state.visible = true;
	widget->state.clickable = oxy_ReturnClickableByType(type);
	widget->update = oxy_ReturnUpdateByType(type);
	widget->render = oxy_ReturnRenderByType(type);
	oxy_SetDefaultColors(widget, type);

	// Set up the basic information for the widget based on its type
	switch (type)
	{
	case OXY_BUTTON_TYPE:
	{
		struct oxy_button_t *button = (struct oxy_button_t *)widget;
		button->label = NULL;
		button->sprite = NULL;
	}
	break;

	case OXY_CHECKBOX_TYPE:
	{
		struct oxy_checkbox_t *checkbox = (struct oxy_checkbox_t *)widget;
		checkbox->on = false;
	}
	break;

	case OXY_COLORBOX_TYPE:
	{
		struct oxy_colorbox_t *colorbox = (struct oxy_colorbox_t *)widget;
		colorbox->index = 0;
	}
	break;

	case OXY_DROPDOWN_TYPE:
	{
		struct oxy_dropdown_t *dropdown = (struct oxy_dropdown_t *)widget;
		// dropdown->items = NULL;
		// dropdown->selected = 0;
	}
	break;

	case OXY_HMARGIN_TYPE:
	{
		struct oxy_hmargin_t *hmargin = (struct oxy_hmargin_t *)widget;
		hmargin->alignment = OXY_ALIGN_BOTTOM;
	}
	break;

	case OXY_VMARGIN_TYPE:
	{
		struct oxy_vmargin_t *vmargin = (struct oxy_vmargin_t *)widget;
		vmargin->alignment = OXY_ALIGN_LEFT;
	}
	break;

	case OXY_MENU_TYPE:
	{
		struct oxy_menu_t *menu = (struct oxy_menu_t *)widget;
		menu->text = NULL;
		menu->sprite = NULL;
		menu->selection = 0;
		menu->rows = 0;
		menu->columns = 0;
	}
	break;

	case OXY_RECTANGLE_TYPE:
	{
		struct oxy_rectangle_t *rectangle = (struct oxy_rectangle_t *)widget;
		rectangle->border = false;
		rectangle->filled = false;
		rectangle->round = false;
	}
	break;

	case OXY_SCROLLBAR_TYPE:
	{
		struct oxy_scrollbar_t *scrollbar = (struct oxy_scrollbar_t *)widget;
		scrollbar->min_value = 0;
		scrollbar->max_value = 100;
		scrollbar->current_value = 0;
		scrollbar->alignment = false;
	}
	break;

	case OXY_SLIDER_TYPE:
	{
		struct oxy_slider_t *slider = (struct oxy_slider_t *)widget;
		slider->render_value = true;
		slider->alignment = true;
		slider->min = 0;
		slider->max = 100;
		slider->curr_value = 0;
		slider->step = 1;
	}
	break;

	case OXY_SPRITE_TYPE:
	{
		struct oxy_sprite_t *sprite = (struct oxy_sprite_t *)widget;
		sprite->spr = NULL;
		sprite->x_scale = 1;
		sprite->y_scale = 1;
		sprite->transparent = false;
	}
	break;

	case OXY_SWITCH_TYPE:
	{
		struct oxy_switch_t *switch_ = (struct oxy_switch_t *)widget;
		switch_->value = false;
		widget->size.width = OXY_DEFAULT_SWITCH_WIDTH;
		widget->size.height = OXY_DEFAULT_SWITCH_HEIGHT;
	}
	break;

	case OXY_LABEL_TYPE:
	{
		struct oxy_label_t *label = (struct oxy_label_t *)widget;
		label->text = NULL;
		label->start_line = 0;
		label->max_lines = 0;
		label->wrap = false;
		widget->size.width = gfx_GetStringWidth(label->text);
		widget->size.height = 9;
	}
	break;

	case OXY_ENTRY_TYPE:
	{
		struct oxy_entry_t *entry = (struct oxy_entry_t *)widget;
		entry->text = NULL;
		entry->cursor_pos = 0;
		entry->max_chars = 0;
		entry->password = false;
		entry->select_start = -1;
		entry->select_end = -1;
	}
	break;

	case OXY_WINDOW_TYPE:
	{
		struct oxy_window_t *window = (struct oxy_window_t *)widget;
		// window->child = NULL;
		window->closeable = true;
		window->minimizable = true;
		window->on_close = NULL;
		window->close_button = NULL;
		window->minimize_button = NULL;
		window->maximize_button = NULL;
	}
	break;

	default:
	{
		widget->size.width = 20;
		widget->size.height = 20;
	}
	break;
	}

	return 1;
}

static void oxy_FreeWidgetByType(struct oxy_widget_t **widget, uint8_t type)
{
	switch (type)
	{

	case OXY_BUTTON_TYPE:
	{
		struct oxy_button_t *button = (struct oxy_button_t *)widget;
		if (button->label)
		{
			free(button->label);
			button->label = NULL;
		}

		if (button->sprite)
		{
			free(button->sprite);
			button->sprite = NULL;
		}
	}
	break;

	
	case OXY_WINDOW_TYPE:
		struct oxy_window_t *window = (struct oxy_window_t *)widget;

		/* Free Windows Buttons */
		if (window->close_button)
		{
			oxy_FreeWidget(window->close_button);
			window->close_button = NULL;
		}

		if (window->minimize_button)
		{
			oxy_FreeWidget(window->minimize_button);
			window->minimize_button = NULL;
		}

		if (window->maximize_button)
		{
			oxy_FreeWidget(window->maximize_button);
			window->maximize_button = NULL;
		}

		break;

	default:
		break;
	}
}

bool oxy_FreeWidget(struct oxy_widget_t **widget)
{
	int i = 0;

	/* Check if there isn't anything in the widget */
	if (widget == NULL)
	{
		return false;
	}

	/* Handle Special Cases */
	oxy_FreeWidgetByType(widget, (*widget)->type);

	/* Free Children's of Parent Child */
	while ((*widget)->child && (*widget)->child[i])
	{
		oxy_FreeWidget(&((*widget)->child[i]));
		i++;
	}

	/* Free Children */
	if ((*widget)->child)
	{
		free((*widget)->child);
		(*widget)->child = NULL;
	}

	/* Free Parent */
	free(*widget);
	(*widget)->child = NULL;

	return true;
}

void oxy_InitConfig(struct oxy_widget_t **stack, struct oxy_color_t *theme)
{
	oxy_config->stack = stack;
	oxy_config->theme = theme;
}

// --------

void oxy_SetWidgetXalign(struct oxy_widget_t *widget, uint8_t alignment)
{
	struct oxy_label_t *label = (struct oxy_label_t *)widget;
	label->widget.position.x_align = alignment;
}

void oxy_SetWidgetYalign(struct oxy_widget_t *widget, uint8_t alignment)
{
	struct oxy_label_t *label = (struct oxy_label_t *)widget;
	label->widget.position.y_align = alignment;
}

void oxy_SetWidgetSize(struct oxy_widget_t *widget, uint16_t width, uint8_t height)
{
	widget->size.width = width;
	widget->size.height = height;
}

void oxy_SetWidgetPosition(struct oxy_widget_t *widget, int x, int y)
{
	if (!widget)
		return;

	int x_shift = x - widget->position.x;
	int y_shift = y - widget->position.y;

	widget->position.x = x;
	widget->position.y = y;

	if (widget->child)
	{
		for (struct oxy_widget_t **w = widget->child; *w; ++w)
		{
			struct oxy_widget_t *p = *w;
			oxy_SetWidgetPosition(p, p->position.x + x_shift, p->position.y + y_shift);
		}
	}
}

void oxy_SetWidgetCallback(struct oxy_widget_t *widget, void (*render)(struct oxy_widget_t *), void (*update)(struct oxy_widget_t *))
{
	widget->update = update;
	widget->render = render;
}

void oxy_SetWidgetChildren(struct oxy_widget_t *widget, struct oxy_widget_t **children)
{
	widget->child = children;
}

// --------

void oxy_AlignWidgetToWidget(struct oxy_widget_t *src_widget, struct oxy_widget_t *dest_widget, uint8_t x_align, uint8_t y_align)
{
	src_widget->position.x = dest_widget->position.x;
	src_widget->position.y = dest_widget->position.y;

	if (x_align == OXY_ALIGN_RIGHT)
	{ // X aligned to Right
		src_widget->position.x += ((dest_widget->size.width - src_widget->size.width) % 2);
	}
	else
	{
		src_widget->position.x = dest_widget->position.x + ((dest_widget->size.width - src_widget->size.width) / 2) * x_align;
	}

	if (y_align == OXY_ALIGN_BOTTOM)
	{ // Y aligned to Bottom
		src_widget->position.y += ((dest_widget->size.height - src_widget->size.height) % 2);
	}
	else
	{
		src_widget->position.y = dest_widget->position.y + ((dest_widget->size.height - src_widget->size.height) / 2) * y_align;
	}
}

void oxy_AlignChildren(struct oxy_widget_t *widget)
{
	if (!widget || widget->type == OXY_MENU_TYPE)
		return;

	for (struct oxy_widget_t **child = widget->child; *child; ++child)
	{
		struct oxy_widget_t *p = *child;

		oxy_AlignWidgetToWidget(p, widget, p->position.x_align, p->position.y_align);

		if (p->type != OXY_MENU_TYPE && p->child)
			oxy_AlignChildren(p);
	}
}

// --------
// Sets everything in the stack to redraw.
void oxy_StackForceRedraw(struct oxy_widget_t **stack)
{

	for (struct oxy_widget_t **w = stack; *w; ++w)
	{
		struct oxy_widget_t *p = *w;

		p->state.redraw = true;

		if (p->child)
			oxy_StackForceRedraw(p->child);
	}
}

// Compares and find items in stack to see if item should be redraw or visible
void oxy_StackCheckRedraw(struct oxy_widget_t **stack, struct oxy_widget_t *reference)
{
	if (!stack || !reference)
		return;

	// Iterate through the stack
	for (struct oxy_widget_t **w = stack; *w; ++w)
	{
		struct oxy_widget_t *p = *w;

		if (p == reference)
			continue;

		// Check if the widget is within the viewable area of the reference widget
		if (oxy_CheckWidgetOverlap(p, reference))
		{
			// Mark the widget for redrawing
			p->state.redraw = true;
		}
		else
		{
			p->state.redraw = false;
		}

		// Recursively check the widget's children
		if (p->child)
			oxy_StackCheckRedraw(p->child, reference);
	}
}

// Removes an object from the stack
void oxy_PopStack(struct oxy_widget_t **stack, struct oxy_widget_t *widget)
{
	if (!stack || !widget)
		return;

	int index = oxy_GetWidgetStackIndex(stack, widget);

	// Return if the widget is not found in the stack
	if (index < 0)
		return;

	// Shift all elements after the widget one position back
	while (stack[index])
	{
		stack[index] = stack[index + 1];
		index++;
	}

	// Set the last element in the stack to NULL
	stack[index] = NULL;
}

// Adds an object to the stack
void oxy_PushStack(struct oxy_widget_t **stack, struct oxy_widget_t *widget, int pos)
{

	// Make sure that the position is within the bounds of the stack
	if (!stack || pos < 0 || pos > oxy_GetStackLength(stack))
		return;

	// Make sure that "stack" has enough space to insert new object
	int len = oxy_GetStackLength(stack);

	// Shift elements to the right starting from pos
	for (int i = len; i > pos; i--)
	{
		stack[i] = stack[i - 1];
	}

	// Insert new element at pos
	stack[pos] = widget;
}

// Returns the index of widget in stack if widget is not found it will return a -1
int oxy_GetWidgetStackIndex(struct oxy_widget_t **stack, struct oxy_widget_t *widget)
{
	int i = 0;

	for (struct oxy_widget_t **w = stack; *w; ++w, ++i)
	{
		if (*w == widget)
		{
			return i;
		}
	}

	return -1;
}

// Retuns the length of the stack
uint16_t oxy_GetStackLength(struct oxy_widget_t **stack)
{
	uint16_t length = 0;

	for (struct oxy_widget_t **w = stack; *w; ++w)
	{
		length++;
	}

	return length;
}

// --------

bool oxy_CheckCursorOverlap(struct oxy_widget_t *dest)
{
	return gfx_CheckRectangleHotspot(CURSOR_X_POS, CURSOR_Y_POS, CURSOR_WIDTH, CURSOR_HEIGHT,
									 dest->position.x, dest->position.y, dest->size.width, dest->size.height);
}

bool oxy_CheckWidgetOverlap(struct oxy_widget_t *src, struct oxy_widget_t *dest)
{
	return gfx_CheckRectangleHotspot(src->position.x, src->position.y, src->size.width, src->size.height,
									 dest->position.x, dest->position.y, dest->size.width, dest->size.height);
}