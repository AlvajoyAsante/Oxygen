#include "oxy_cursor.h"
#include "gfx/oxy_sprites.h"

#include <tice.h>
#include <string.h>
#include <graphx.h>
#include <keypadc.h>

struct oxy_cursor_t oxy_cursor;
struct oxy_cursor_info_t oxy_cursor_temp;

/* Setting up the Cursor */
void oxy_InitCursor(void)
{
	// Setting up sprites
	oxy_cursor.spr[0] = cursorA;
	oxy_cursor.spr[1] = cursorB;
	oxy_cursor.spr[2] = cursorC;
	oxy_cursor.spr[3] = cursorD;
	oxy_cursor.spr[4] = cursorE;

	// Setting up cursor position
	oxy_cursor.x = (LCD_WIDTH - cursorA_width) / 2;
	oxy_cursor.y = (LCD_HEIGHT - cursorA_height) / 2;

	// Setting up the speed of the cursor and the state of the cursor
	oxy_cursor.speed = 4;
	oxy_cursor.state = CURSOR_STATE_IDLE;

	// Setting up the scrolling position (which should be '0')
	oxy_cursor.scroll_X = oxy_cursor.scroll_Y = 0;

	// Reset the cursor click id
	oxy_cursor.clicked_id = oxy_cursor.hover_id = CURSOR_RESET_ID;

	// Settting up double buffering
	CURSOR_BACK_BUFFER = gfx_MallocSprite(CURSOR_WIDTH, CURSOR_HEIGHT);
}

void oxy_SetCursorTempCallBack(struct oxy_widget_t *widget)
{
	// Set the temp description to set widget description
	oxy_cursor_temp.description = widget->cursor_info.description;

	// Set the temp hover callback to set widget hover callback
	oxy_cursor_temp.hover = widget->cursor_info.hover;
	oxy_cursor_temp.hover_arg = widget->cursor_info.hover_arg;

	// Set the temp left_click callback to set widget left_click callback
	oxy_cursor_temp.left_click = widget->cursor_info.left_click;
	oxy_cursor_temp.left_arg = widget->cursor_info.left_arg;

	// Set the temp right_click callback to set widget right_click callback
	oxy_cursor_temp.right_click = widget->cursor_info.right_click;
	oxy_cursor_temp.right_arg = widget->cursor_info.right_arg;
}

void oxy_ResetCursorTempCallBack(void)
{
	// Reset description
	oxy_cursor_temp.description = NULL;

	// Reset hover
	oxy_cursor_temp.hover = NULL;
	oxy_cursor_temp.hover_arg = NULL;

	// Reset left_click
	oxy_cursor_temp.left_click = NULL;
	oxy_cursor_temp.left_arg = NULL;

	// Reset right_click
	oxy_cursor_temp.right_click = NULL;
	oxy_cursor_temp.right_arg = NULL;
}

void oxy_ResetCursor(void)
{
	// Resets the scrolling positions
	oxy_cursor.scroll_X = oxy_cursor.scroll_Y = 0;

	// Reset the cursor state to IDLE
	oxy_cursor.state = CURSOR_STATE_IDLE;

	// Reset the cursor click id
	oxy_cursor.clicked_id = oxy_cursor.hover_id = CURSOR_RESET_ID;
}

/* Setting Cursor Variables */
bool oxy_SetCursorSpeed(uint8_t speed)
{
	// Makes sure that cursor speed is width in limits.
	if (speed <= 10 && speed >= 1)
	{
		CURSOR_SPEED = speed; // The cursor speed was set.
		return true;
	}

	return false;
}

bool oxy_CheckCursorForcedExit(void)
{
	// retuns a bool if the clicked id equals to to the forced exit ID
	return (bool)(oxy_cursor.clicked_id == CURSOR_FORCE_EXIT_ID);
}

void oxy_CenterCursor(void)
{
	// Simple centering positioning formula.
	CURSOR_X_POS = (LCD_WIDTH - CURSOR_WIDTH) / 2;
	CURSOR_Y_POS = (LCD_HEIGHT - CURSOR_HEIGHT) / 2;
}

void oxy_RenderCursorSprite(void)
{
	gfx_SetTransparentColor(0xf0);
	switch (oxy_cursor.state)
	{
	case CURSOR_STATE_IDLE:
		gfx_TransparentSprite(CURSOR_SPRITE_IDLE, CURSOR_X_POS, CURSOR_Y_POS);
		break;

	case CURSOR_STATE_CLICK:
		gfx_TransparentSprite(CURSOR_SPRITE_CLICK, CURSOR_X_POS, CURSOR_Y_POS);
		break;

	case CURSOR_STATE_MOVE:
		gfx_TransparentSprite(CURSOR_SPRITE_MOVE, CURSOR_X_POS, CURSOR_Y_POS);
		break;

	case CURSOR_STATE_H_MOVE:
		gfx_TransparentSprite(CURSOR_SPRITE_H_MOVE, CURSOR_X_POS, CURSOR_Y_POS);
		break;

	case CURSOR_STATE_V_MOVE:
		gfx_TransparentSprite(CURSOR_SPRITE_V_MOVE, CURSOR_X_POS, CURSOR_Y_POS);
		break;

	default: // if the state is not detected the just display the idle cursor (change later! maybe an error cursor?)
		gfx_TransparentSprite(CURSOR_SPRITE_IDLE, CURSOR_X_POS, CURSOR_Y_POS);
		break;
	}
}

bool oxy_RenderCursorBackBuff(void)
{
	if (!CURSOR_BACK_BUFFER)
		return 0;

	gfx_Sprite(CURSOR_BACK_BUFFER, CURSOR_X_POS, CURSOR_Y_POS);
	return 1;
}

bool oxy_GetCursorBackBuff(void)
{
	if (!CURSOR_BACK_BUFFER)
		return 0;

	gfx_GetSprite(CURSOR_BACK_BUFFER, CURSOR_X_POS, CURSOR_Y_POS);
	return 1;
}

void oxy_UpdateCursor(void)
{
	kb_key_t key;

	kb_Scan();

	key = kb_Data[7];

	// Cursor Force Exit
	if (kb_Data[6] & kb_Clear)
	{
		oxy_cursor.clicked_id = -3; // Returns a -3 for Clear clicked
		return;
	}

	// Cursor Clicking
	if ((kb_Data[6] & kb_Enter || kb_Data[1] & kb_2nd))
	{ // Right Click
		if (CURSOR_TEMP_RIGHT_CLICK != NULL)
		{
			if (CURSOR_TEMP_RIGHT_ARG != NULL)
			{
				CURSOR_TEMP_RIGHT_CLICK(CURSOR_TEMP_RIGHT_ARG);
			}
			else
			{
				CURSOR_TEMP_RIGHT_CLICK(NULL);
			}
		}
		return;
	}

	if ((kb_Data[1] & kb_KeyAlpha) && (oxy_cursor.hover_id != -1))
	{ // Left Click
		if (CURSOR_TEMP_LEFT_CLICK != NULL)
		{
			if (CURSOR_TEMP_LEFT_ARG != NULL)
			{
				CURSOR_TEMP_LEFT_CLICK(CURSOR_TEMP_LEFT_ARG);
			}
			else
			{
				CURSOR_TEMP_LEFT_CLICK(NULL);
			}
		}
		return;
	}

	// Cursor Speed Change
	if (kb_IsDown(kb_KeyMode) && kb_IsDown(kb_KeyUp))
	{
		if (CURSOR_SPEED != 10)
			CURSOR_SPEED++; // Increases oxy_cursor speed.
		return;
	}

	if (kb_IsDown(kb_KeyMode) && kb_IsDown(kb_KeyDown))
	{
		if (CURSOR_SPEED != 1)
			CURSOR_SPEED--; // Decrease oxy_cursor speed.
		return;
	}

	// Cursor Scrolling
	if (kb_IsDown(kb_KeyAlpha) && kb_IsDown(kb_KeyUp))
	{
		if (CURSOR_SCROLL_Y != 0)
		{
			CURSOR_SCROLL_Y -= 1;
		}
		return;
	}

	if (kb_IsDown(kb_KeyAlpha) && kb_IsDown(kb_KeyLeft))
	{
		if (CURSOR_SCROLL_X != 0)
		{
			CURSOR_SCROLL_X -= 1;
		}
		return;
	}

	if (kb_IsDown(kb_KeyAlpha) && kb_IsDown(kb_KeyDown))
	{
		CURSOR_SCROLL_Y += 1;
		return;
	}

	if (kb_IsDown(kb_KeyAlpha) && kb_IsDown(kb_KeyRight))
	{
		CURSOR_SCROLL_X += 1;
		return;
	}

	// Cursor Movement Keys
	switch (key)
	{
	case kb_Up:
		if (CURSOR_Y_POS - CURSOR_SPEED > -2)
			CURSOR_Y_POS -= CURSOR_SPEED;
		break;

	case kb_Down:
		if (CURSOR_Y_POS + CURSOR_SPEED <= LCD_HEIGHT)
			CURSOR_Y_POS += CURSOR_SPEED;
		break;

	case kb_Left:
		if (CURSOR_X_POS - CURSOR_SPEED > -2)
			CURSOR_X_POS -= CURSOR_SPEED;
		break;

	case kb_Right:
		if (CURSOR_X_POS + CURSOR_SPEED <= LCD_WIDTH)
			CURSOR_X_POS += CURSOR_SPEED;
		break;
	}

	if (kb_IsDown(kb_KeyUp) && kb_IsDown(kb_KeyRight))
	{
		if (CURSOR_Y_POS - CURSOR_SPEED > -2)
			CURSOR_Y_POS -= CURSOR_SPEED;

		if (CURSOR_X_POS + CURSOR_SPEED <= LCD_WIDTH)
			CURSOR_X_POS += CURSOR_SPEED;
	}

	if (kb_IsDown(kb_KeyUp) && kb_IsDown(kb_KeyLeft))
	{
		if (CURSOR_Y_POS - CURSOR_SPEED > -2)
			CURSOR_Y_POS -= CURSOR_SPEED;

		if (CURSOR_X_POS - CURSOR_SPEED > -2)
			CURSOR_X_POS -= CURSOR_SPEED;
	}

	if (kb_IsDown(kb_KeyDown) && kb_IsDown(kb_KeyRight))
	{
		if (CURSOR_Y_POS + CURSOR_SPEED <= LCD_HEIGHT)
			CURSOR_Y_POS += CURSOR_SPEED;

		if (CURSOR_X_POS + CURSOR_SPEED <= LCD_WIDTH)
			CURSOR_X_POS += CURSOR_SPEED;
	}

	if (kb_IsDown(kb_KeyDown) && kb_IsDown(kb_KeyLeft))
	{
		if (CURSOR_Y_POS + CURSOR_SPEED <= LCD_HEIGHT)
			CURSOR_Y_POS += CURSOR_SPEED;

		if (CURSOR_X_POS - CURSOR_SPEED > -2)
			CURSOR_X_POS -= CURSOR_SPEED;
	}
}

void oxy_RenderCursor(void)
{
	uint16_t des_x;
	uint8_t des_y;
	gfx_sprite_t *text_buff = NULL;

	oxy_ResetCursor(); // reset last clicked index
	
	if (!oxy_GetCursorBackBuff()) return;

	oxy_RenderCursorSprite();

	if (CURSOR_STATE_ID != CURSOR_STATE_IDLE)
	{
		// Run a function if the cursor is in a click state
		if (CURSOR_TEMP_HOVER)
		{
			if (CURSOR_TEMP_HOVER_ARG)
			{
				CURSOR_TEMP_HOVER(CURSOR_TEMP_HOVER_ARG);
			}
			else
			{
				CURSOR_TEMP_HOVER(NULL);
			}
		}

		// Printing widget description
		if (CURSOR_TEMP_DESCRIPTION)
		{
			text_buff = gfx_MallocSprite(gfx_GetStringWidth(CURSOR_TEMP_DESCRIPTION), 9);

			if (CURSOR_X_POS <= LCD_WIDTH / 2)
				des_x = CURSOR_X_POS + CURSOR_WIDTH + 1;

			if (CURSOR_X_POS > LCD_WIDTH / 2)
				des_x = CURSOR_X_POS - gfx_GetStringWidth(CURSOR_TEMP_DESCRIPTION) - 8;

			if (CURSOR_Y_POS <= LCD_HEIGHT / 2)
				des_y = CURSOR_Y_POS + CURSOR_HEIGHT + 1;

			if (CURSOR_Y_POS > LCD_HEIGHT / 2)
				des_y = CURSOR_Y_POS + 1;

			gfx_GetSprite(text_buff, des_x, des_y);
			gfx_PrintStringXY(CURSOR_TEMP_DESCRIPTION, des_x, des_y);
		}
	}

	gfx_Blit(1);

	oxy_RenderCursorBackBuff();

	if (text_buff)
	{
		gfx_Sprite(text_buff, des_x, des_y);
		free(text_buff);
	}

	kb_Scan();
}
