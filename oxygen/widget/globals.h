/**
 * @file global.h
 *
 * @author Alvajoy "Alvajoy123" Asante
 * @version 1.0
 * @copyright Copyright (c) 2018 - 2023	
 *
 *      _/_/    _/      _/  _/      _/    _/_/_/  _/_/_/_/  _/      _/
 *   _/    _/    _/  _/      _/  _/    _/        _/        _/_/    _/
 *  _/    _/      _/          _/      _/  _/_/  _/_/_/    _/  _/  _/
 * _/    _/    _/  _/        _/      _/    _/  _/        _/    _/_/
 *  _/_/    _/      _/      _/        _/_/_/  _/_/_/_/  _/      _/
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef GLOBALS_H
#define GLOBALS_H

#include <tice.h>

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

	/**
	 * @brief alignment used by widgets.
	 */
	enum oxy_align_t
	{
		OXY_ALIGN_LEFT,
		OXY_ALIGN_CENTERED,
		OXY_ALIGN_RIGHT,
		OXY_ALIGN_TOP = 0,
		OXY_ALIGN_BOTTOM = 2,
	};

	/**
	 * @brief Types of widgets.
	 */
	enum oxy_type_t
	{
		OXY_BUTTON_TYPE,
		OXY_CHECKBOX_TYPE,
		OXY_COLORBOX_TYPE,
		OXY_DROPDOWN_TYPE,
		OXY_HMARGIN_TYPE,
		OXY_MENU_TYPE,
		OXY_RECTANGLE_TYPE,
		OXY_SCROLLBAR_TYPE,
		OXY_SLIDER_TYPE,
		OXY_SPRITE_TYPE,
		OXY_SWITCH_TYPE,
		OXY_LABEL_TYPE,
		OXY_ENTRY_TYPE,
		OXY_VMARGIN_TYPE,
		OXY_WINDOW_TYPE,
	};

	/**
	 * @brief Color of GTK objects.
	 */
	struct oxy_color_t
	{
		uint8_t text_fg_unselected; // Text foreground color for unselected widgets
		uint8_t text_bg_unselected; // Text background color for unselected widgets
		uint8_t unselected;			// Genreal unselected color

		uint8_t text_fg_selected; // Text foreground color for selected widgets
		uint8_t text_bg_selected; // Text Background color for selected widgets
		uint8_t selected;		  // Genreal selected color

		uint8_t text_fg_clicked; // Text foreground color for clicked widgets
		uint8_t text_bg_clicked; // Text background color for clicked widgets
		uint8_t clicked;		 // Genreal clicked color

		uint8_t color_a; // Extra color #1
		uint8_t color_b; // Extra color #2
		uint8_t color_c; // Extra color #3
	};

	/**
	 * @brief State of an object "derived from GTK".
	 */
	struct oxy_state_t
	{
		bool visible;	// Used to check if widget can be seen on screen.
		bool clickable; // Used to check if widget can be clicked.
		bool clicked;	// Used to check if it's been clicked.
		bool selected;	// Used to check if cursor is hovering over widget.
		bool redraw;	// Used to check if widget needs to redrawn.
	};

	/**
	 * @brief Structure that contains Position and offsets.
	 */
	struct oxy_position_t
	{
		int x;			 // X position.
		int y;			 // Y position.
		uint8_t x_align; // (X) Horizontal align type.
		uint8_t y_align; // (Y) Vertical align type.
	};

	/**
	 * @brief Structure that contains width and height data.
	 */
	struct oxy_sizeinfo_t
	{
		uint16_t width; // Width of widget.
		uint8_t height; // Height of widget.
	};

	/**
	 * @brief Structure that contains all information for resizing.
	 */
	struct oxy_resizeinfo_t
	{
		bool resizable; // Used to check if widget can be resized.

		uint16_t min_width; // Minimum width.
		uint8_t min_height; // Minimum height.
		uint16_t max_width; // Max width.
		uint8_t max_height; // Max height.

		struct oxy_position_t pos_reference;  // Used to check position reference
		struct oxy_sizeinfo_t size_reference; // Used to check size reference
	};

	/**
	 * @brief Structure that contains information to add more features to oxygen cursor.
	 */
	struct oxy_cursor_info_t
	{
		char *description; // Text that appears when hovering.

		void (*hover)(void *); // callback for hovering (selected).
		void *hover_arg;	   // arguments for "hovering".

		void (*left_click)(void *); // callback for left clicking.
		void *left_arg;				// arguments for "right click".

		void (*right_click)(void *); // callback for right clicking.
		void *right_arg;			 // arguments for "left click".
	};

	/**
	 * @brief Base structure for displayable objects.
	 */
	struct oxy_widget_t
	{
		uint8_t type; // Used to determine the type of widget.

		/* Call Back Functions */
		struct oxy_widget_t **child;		   // Used to point other widgets.
		void (*update)(struct oxy_widget_t *); // Used for updating widget if needed.
		void (*render)(struct oxy_widget_t *); // Used for rendering widget.

		/* Basic Widget Information */
		struct oxy_color_t color;			  // Allows widgets to have color.
		struct oxy_position_t position;		  // Allows widgets to have positions.
		struct oxy_sizeinfo_t size;			  // Allows widgets to have size variations.
		struct oxy_resizeinfo_t resize;		  // Allows widgets to be resizable.
		struct oxy_state_t state;			  // Allows widgets to work with oxygen cursor.
		struct oxy_cursor_info_t cursor_info; // Allows widgets to use more oxygen features.
	};

	/**
	 * @brief Rendering Configuration for GUI
	 */
	struct oxy_render_config_t
	{
		bool needs_redraw;
		// bool installed_font;
		uint16_t tick;
	};

	/**
	 * @brief Configuration structure for wiget elements
	 * @note This is basically the settings for the framework
	 */
	struct oxy_config_t
	{
		struct oxy_widget_t **stack;	   // Manages all the widgets in stack can
		struct oxy_color_t *theme;		   // Maintains a overall color scheme
		struct oxy_render_config_t render; // Manages rendering information
	};
	extern struct oxy_config_t *oxy_config;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* GLOBALS_H */