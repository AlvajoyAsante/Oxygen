#include "color.h"

void oxy_SetWidgetColor(struct oxy_widget_t *widget, uint8_t color_a, uint8_t color_b)
{
	widget->color.color_a = color_a;
	widget->color.color_b = color_b;
}

void oxy_SetWidgetUnselectedColor(struct oxy_widget_t *widget, uint8_t color, uint8_t text_fg, uint8_t text_bg)
{
	widget->color.text_fg_unselected = text_fg;
	widget->color.text_bg_unselected = text_bg;
	widget->color.unselected = color;
}

void oxy_SetWidgetSelectedColor(struct oxy_widget_t *widget, uint8_t color, uint8_t text_fg, uint8_t text_bg)
{
	widget->color.text_fg_selected = text_fg;
	widget->color.text_bg_selected = text_bg;
	widget->color.selected = color;
}

void oxy_SetWidgetClickedColor(struct oxy_widget_t *widget, uint8_t color, uint8_t text_fg, uint8_t text_bg)
{
	widget->color.text_fg_clicked = text_fg;
	widget->color.text_bg_clicked = text_bg;
	widget->color.clicked = color;
}

void oxy_SetDefaultColors(struct oxy_widget_t *widget, uint8_t type)
{
	switch (type)
	{
	case OXY_BUTTON_TYPE:
		widget->color.text_fg_unselected = OXY_BUTTON_TEXT_FG_UNSELECTED_COLOR;
		widget->color.text_bg_unselected = OXY_BUTTON_TEXT_BG_UNSELECTED_COLOR;
		widget->color.unselected = OXY_BUTTON_BG_UNSELECTED_COLOR;

		widget->color.text_fg_selected = OXY_BUTTON_TEXT_FG_SELECTED_COLOR;
		widget->color.text_bg_selected = OXY_BUTTON_TEXT_BG_SELECTED_COLOR;
		widget->color.selected = OXY_BUTTON_BG_SELECTED_COLOR;

		widget->color.text_fg_clicked = OXY_BUTTON_TEXT_FG_CLICKED_COLOR;
		widget->color.text_bg_clicked = OXY_BUTTON_TEXT_BG_CLICKED_COLOR;
		widget->color.clicked = OXY_BUTTON_BG_CLICKED_COLOR;

		widget->color.color_a = OXY_BUTTON_FILL_COLOR;	 // FILL COLOR
		widget->color.color_b = OXY_BUTTON_BORDER_COLOR; // OUTLINE COLOR #1
		break;

	case OXY_WINDOW_TYPE:
		widget->color.text_fg_unselected = OXY_WINDOW_TITLE_TEXT_FG_UNSELECTED_COLOR;
		widget->color.text_bg_unselected = OXY_WINDOW_TITLE_TEXT_BG_UNSELECTED_COLOR;
		widget->color.unselected = OXY_WINDOW_UNSELECTED_COLOR;

		widget->color.text_fg_selected = OXY_WINDOW_TITLE_TEXT_FG_SELECTED_COLOR;
		widget->color.text_bg_selected = OXY_WINDOW_TITLE_TEXT_BG_SELECTED_COLOR;
		widget->color.selected = OXY_WINDOW_SELECTED_COLOR;

		widget->color.color_a = OXY_WINDOW_FILL_COLOR;	   // FILL COLOR
		widget->color.color_b = OXY_WINDOW_BORDER_COLOR_A; // OUTLINE COLOR #1
		widget->color.color_c = OXY_WINDOW_BORDER_COLOR_B; // OUTLINE COLOR #2
		break;

	default:
		widget->color.text_fg_unselected = OXY_DEFAULT_TEXT_FG_COLOR;
		widget->color.text_bg_unselected = OXY_DEFAULT_TEXT_BG_COLOR;
		widget->color.unselected = OXY_DEFAULT_UNSELECTED_COLOR;

		widget->color.text_fg_selected = OXY_DEFAULT_TEXT_FG_COLOR;
		widget->color.text_bg_selected = OXY_DEFAULT_TEXT_BG_COLOR;
		widget->color.selected = OXY_DEFAULT_SELECTED_COLOR;

		widget->color.text_fg_clicked = OXY_DEFAULT_TEXT_FG_COLOR;
		widget->color.text_bg_clicked = OXY_DEFAULT_TEXT_BG_COLOR;
		widget->color.clicked = OXY_DEFAULT_CLICKED_COLOR;

		widget->color.color_a = OXY_DEFAULT_COLOR_A_COLOR; // FILL COLOR
		widget->color.color_b = OXY_DEFAULT_COLOR_B_COLOR; // OUTLINE COLOR #1
		widget->color.color_c = OXY_DEFAULT_COLOR_C_COLOR; // OUTLINE COLOR #2
		break;
	}
}

struct oxy_color_t oxy_GetWidgetColor(struct oxy_widget_t *widget)
{
	return widget->color;
}
