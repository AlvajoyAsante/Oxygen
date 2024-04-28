/**
 * @file oxy_cursor.h
 * @brief This file contains functions for cursor
 *
 * @author Alvajoy "Alvajoy123" Asante
 * @version 1.0
 * @copyright Copyright (c) 2018 - 2024
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

#ifndef OXY_CURSOR_H
#define OXY_CURSOR_H

#define CURSOR_WIDTH 17
#define CURSOR_HEIGHT 22

#define CURSOR_X_POS oxy_cursor.x
#define CURSOR_Y_POS oxy_cursor.y

#define CURSOR_SPRITE_IDLE oxy_cursor.spr[0]
#define CURSOR_SPRITE_CLICK oxy_cursor.spr[1]
#define CURSOR_SPRITE_MOVE oxy_cursor.spr[2]
#define CURSOR_SPRITE_H_MOVE oxy_cursor.spr[3]
#define CURSOR_SPRITE_V_MOVE oxy_cursor.spr[4]

#define CURSOR_BACK_BUFFER oxy_cursor.back

#define CURSOR_HOVER_ID oxy_cursor.hover_id
#define CURSOR_CLICKED_ID oxy_cursor.clicked_id
#define CURSOR_STATE_ID oxy_cursor.state

#define CURSOR_SCROLL_X oxy_cursor.scroll_X
#define CURSOR_SCROLL_Y oxy_cursor.scroll_Y

#define CURSOR_SPEED oxy_cursor.speed

#define CURSOR_TEMP_HOVER oxy_cursor_temp.hover
#define CURSOR_TEMP_HOVER_ARG oxy_cursor_temp.hover_arg

#define CURSOR_TEMP_LEFT_CLICK oxy_cursor_temp.left_click
#define CURSOR_TEMP_LEFT_ARG oxy_cursor_temp.left_arg

#define CURSOR_TEMP_RIGHT_CLICK oxy_cursor_temp.right_click
#define CURSOR_TEMP_RIGHT_ARG oxy_cursor_temp.right_arg

#define CURSOR_TEMP_DESCRIPTION oxy_cursor_temp.description

#define CURSOR_RESET_ID -2
#define CURSOR_FORCE_EXIT_ID -3

#include <tice.h>
#include <graphx.h>
#include "widget/globals.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
	/**
	 * @brief An enumeration of the different states that the cursor can be in
	 *
	 * @note
	 * The CURSOR_STATE_IDLE state represents the cursor being in a neutral, idle position.
	 * The CURSOR_STATE_CLICK state represents the cursor being in a clicking position.
	 * The CURSOR_STATE_MOVE state represents the cursor being in a moving position.
	 * The CURSOR_STATE_H_MOVE state represents the cursor being in a horizontal moving position.
	 * The CURSOR_STATE_V_MOVE state represents the cursor being in a vertical moving position.
	 */
	enum oxy_cursor_state_t
	{
		CURSOR_STATE_IDLE,
		CURSOR_STATE_CLICK,
		CURSOR_STATE_MOVE,
		CURSOR_STATE_H_MOVE,
		CURSOR_STATE_V_MOVE,
	};

	/**
	 * @brief Holds information about Cursor.
	 */
	struct oxy_cursor_t
	{
		int x;
		int y;
		int scroll_X;
		int scroll_Y;

		uint8_t speed;
		enum oxy_cursor_state_t state;

		gfx_sprite_t *spr[5];
		gfx_sprite_t *back;

		int clicked_id;
		int hover_id;
	};
	extern struct oxy_cursor_t oxy_cursor;

	/**
	 * @brief Holds temp information about widget data.
	 */
	extern struct oxy_cursor_info_t oxy_cursor_temp;

	/**
	 * @brief Sets cursor temps callbacks to those of 'widget'
	 * @note This functions is mainly used in widget update functions.
	 * @param  widget:
	 */
	void oxy_SetCursorTempCallBack(struct oxy_widget_t *widget);

	/**
	 * @brief This functions resets the cursor temp call backs
	 * @note This function is mainly used in widget update functions
	 */
	void oxy_ResetCursorTempCallBack(void);

	/**
	 * @brief Init all variable needed for Cursor file.
	 */
	void oxy_InitCursor(void);

	/**
	 * @brief Centers the cursor in middle of the screen.
	 */
	void oxy_CenterCursor(void);

	/**
	 * @brief This function is used to reset the Cursor scroll and clicked index.
	 */
	void oxy_ResetCursor(void);

	/**
	 * Sets the movement speed of the cursor
	 */
	bool oxy_SetCursorSpeed(uint8_t speed);

	/**
	 * @brief This function is used to render the cursor on the back buffer of the screen.
	 */
	bool oxy_RenderCursorBackBuff(void);

	/**
	 * @brief This function is used to get the cursor on the back buffer of the screen.
	 */
	bool oxy_GetCursorBackBuff(void);

	/**
	 * @brief Updates Cursor
	 */
	void oxy_UpdateCursor(void);

	/**
	 * @brief Renders Cursor
	 */
	void oxy_RenderCursor(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif