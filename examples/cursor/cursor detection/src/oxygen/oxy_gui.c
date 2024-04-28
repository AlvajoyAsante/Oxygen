#include "oxy_gui.h"
#include "oxy_gfx.h"
#include "oxy_gtk.h"
#include "gfx/oxy_sprites.h"

#include <tice.h>
#include <graphx.h>
#include <keypadc.h>
#include <debug.h>

struct oxy_stringinput_t oxy_stringinput;

// String input.
static void oxy_ResetStringInput(void)
{
	// Gets string ready for input.
	oxy_stringinput.text = realloc(oxy_stringinput.text, oxy_stringinput.max_char);
	oxy_stringinput.text[0] = '\0';
	oxy_stringinput.charsamount = 1;
	oxy_stringinput.type = 0;

	// Used to check for a forced exit.
	oxy_stringinput.forced_exit = 0;

	// Creates a cursor (Should reset it later on!).
	gfx_SetCharData('|', (uint8_t[]){0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0x00});
}

static void oxy_UpdateString(void)
{
	const char *keys[3] = {"\0\0\0\0\0\0\0\0\0\0\0WRMH\0\0?\0VQLG\0\0.ZUPKFC\0 YTOJEB\0\0XSNIDA\0\0\0\0\0\0\0\0\0",
						   "\0\0\0\0\0\0\0\0\0\0\0wrmh\0\0?\0vqlg\0\0.zupkfc\0 ytojeb\0\0xsnida\0\0\0\0\0\0\0\0\0",
						   "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0-\x33\x36\x39)\0\0\0.\x32\x35\x38(\0\0\0\x30\x31\x34\x37,\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"};
	uint8_t skKey;

	skKey = os_GetCSC();

	if (!skKey)
		return;

	if (keys[oxy_stringinput.type][skKey] && oxy_stringinput.charsamount - 1 != oxy_stringinput.max_char)
	{
		oxy_stringinput.charsamount++;
		oxy_stringinput.text[oxy_stringinput.charsamount - 2] = keys[oxy_stringinput.type][skKey];
	}

	if (skKey == sk_Del && oxy_stringinput.charsamount > 1)
	{
		oxy_stringinput.charsamount--;
		oxy_stringinput.text[oxy_stringinput.charsamount - 1] = '\0';
	}

	if (skKey == sk_Clear)
	{
		if (oxy_stringinput.text[0] == '\0')
		{
			oxy_stringinput.forced_exit = 1;
		}
		else
			oxy_ResetStringInput();
	}

	if (skKey == sk_Alpha)
	{
		if (oxy_stringinput.type + 1 > 2)
		{
			oxy_stringinput.type = 0;
		}
		else
			oxy_stringinput.type++;
	}

	oxy_stringinput.text[oxy_stringinput.charsamount - 1] = '\0';
}

char *oxy_StringInput(const char title[], uint16_t x, uint8_t y, uint8_t maxchar)
{
	char temp[2] = "";
	oxy_stringinput.max_char = maxchar;

	oxy_ResetStringInput();

	while (!(kb_Data[6] & kb_Enter) && !oxy_stringinput.forced_exit)
	{
		oxy_UpdateString();
		kb_Scan();

		gfx_FillRectangle(x, y, (8 * oxy_stringinput.charsamount) * maxchar, 9); // Text buffer.

		switch (oxy_stringinput.type)
		{
		case 0:
			temp[0] = 'A';
			break;

		case 1:
			temp[0] = 'a';
			break;

		case 2:
			temp[0] = '1';
			break;
		}

		/* Rendering inputted text */
		gfx_FillRectangle(x, y, (8 * oxy_stringinput.charsamount) * maxchar, 9); // Text buffer.

		gfx_SetTextXY(x, y);
		if (title != NULL)
			gfx_PrintString(title);
		gfx_PrintString(oxy_stringinput.text); // Text printed here.

		if (randInt(0, 1))
			gfx_PrintString("|"); // Randomly printing cursor.

		/* Rendering text type */
		gfx_FillRectangle(LCD_WIDTH - (gfx_GetStringWidth(&temp[0]) + 2), 1, 10, 9);
		gfx_PrintStringXY(&temp[0], LCD_WIDTH - (gfx_GetStringWidth(&temp[0]) + 1), 1); // Text type printed here.

		gfx_Blit(1);
	}

	/* Removing Cursor */
	gfx_FillRectangle(gfx_GetStringWidth(title) + gfx_GetStringWidth(oxy_stringinput.text), y, gfx_GetStringWidth("|"), 8);
	gfx_Blit(1);

	return oxy_stringinput.text;
}

// Others.
uint8_t oxy_ColorPicker(uint8_t cur_select, uint16_t x, uint8_t y)
{
	uint8_t color;
	uint8_t count = 0;
	color = cur_select;

	while (!((kb_Data[6] & kb_Enter) || (kb_Data[1] & kb_2nd) || (kb_Data[6] & kb_Clear)))
	{
		kb_Scan();
		// keys!
		switch (kb_Data[7])
		{
		case kb_Up:
			color -= 32;
			break;

		case kb_Down:
			color += 32;
			break;

		case kb_Left:
			color--;
			break;

		case kb_Right:
			color++;
			break;
		}

		// Rendering
		count = 0;
		for (int i = 1; i < 9; i++)
		{
			for (int r = 0; r < 32; r++)
			{
				gfx_SetColor(count);
				gfx_FillRectangle(x + ((r % 32) * 8), y + (i * 8) - 8, 8, 8);

				if (count == color)
				{
					gfx_SetColor(255 - color);
					gfx_Rectangle(x + ((r % 32) * 8), y + (i * 8) - 8, 8, 8);
				}

				count++;
			}
		}
	}

	while (kb_AnyKey())
		kb_Scan();

	return color;
}

void oxy_PrintTime(uint16_t x, uint8_t y)
{
	uint8_t sec, min, hur;
	const char *time_ind[2] = {" AM", " PM"};

	gfx_FillRectangle(x, y, 55, 8);

	boot_GetTime(&sec, &min, &hur);

	if (hur == 0)
		hur = 12;

	if (hur > 12)
		hur -= 12;

	gfx_SetTextXY(x, y);
	gfx_PrintUInt((unsigned int)hur, 2);
	gfx_PrintString(":");
	gfx_PrintUInt((unsigned int)min, 2);
	gfx_PrintString(time_ind[(uint8_t)boot_IsAfterNoon()]);
}

void oxy_PrintDate(uint16_t x, uint8_t y)
{
	uint8_t day, month;
	uint16_t year;

	boot_GetDate(&day, &month, &year);

	gfx_FillRectangle(x, y, 57, 8);

	gfx_SetTextXY(x, y);
	gfx_PrintInt((unsigned int)month, 2);
	gfx_PrintString("/");
	gfx_PrintInt((unsigned int)day, 2);
	gfx_PrintString("/");
	gfx_PrintInt((unsigned int)year, 4);
}

void oxy_PrintBatteryStatus(uint16_t x, uint8_t y)
{
	int bat_status;
	bat_status = boot_GetBatteryStatus();

	/* Rendering battery outline */
	gfx_Rectangle(x, y, 13, 10);
	gfx_Rectangle(x + 13, y + 2, 2, 6);

	if (bat_status)
	{
		if (bat_status > 2)
		{
			gfx_SetColor(0x07);
		}
		else
			gfx_SetColor(0xE2);
		bat_status = bat_status * 2.25;
	}
	else
	{
		gfx_SetColor(0xE0);
		bat_status = 1;
	}

	gfx_FillRectangle(x + 2, y + 2, bat_status, 6);
}