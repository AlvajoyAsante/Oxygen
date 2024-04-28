#include "oxy_gui.h"
#include "oxy_gfx.h"
#include "oxy_widget.h"
#include "gfx/oxy_sprites.h"

#include <tice.h>
#include <graphx.h>
#include <keypadc.h>
#include <debug.h>

struct oxy_stringinput_t oxy_stringinput;

// 	Message & Dialogue
void oxy_ResetStringInputHistory(void)
{
	oxy_stringinput.enable_history = 1;
	oxy_stringinput.history_pos = oxy_stringinput.history_size = 0;

	for (int i = 0; i < 256; i++)
	{
		oxy_stringinput.history[i] = NULL;
	}
}

void oxy_ResetStringInputAutofill(void)
{
	oxy_stringinput.enable_autofill = 1;
	oxy_stringinput.autofill_size = 0;

	for (int i = 0; i < 500; i++)
	{
		oxy_stringinput.autofill[i] = NULL;
	}
}

void oxy_StringInput_AddToAutofill(const char *input)
{
	if (!oxy_stringinput.enable_autofill)
	{
		return; // History is not enabled, do nothing
	}

	// Check if the history size has reached its maximum
	if (oxy_stringinput.autofill_size > 499)
	{
		return; // History is full, cannot add more entries
	}

	// Copy the input string into the history at the current position
	oxy_stringinput.autofill[oxy_stringinput.autofill_size] = strdup(input);

	// Increment the history size
	oxy_stringinput.autofill_size++;
}

void oxy_StringInput_AddToHistory(const char *input)
{
	if (!oxy_stringinput.enable_history)
	{
		return; // History is not enabled, do nothing
	}

	// Check if the history size has reached its maximum
	if (oxy_stringinput.history_size > 255)
	{
		return; // History is full, cannot add more entries
	}

	// Copy the input string into the history at the current position
	oxy_stringinput.history[oxy_stringinput.history_size] = strdup(input);

	// Increment the history size
	oxy_stringinput.history_size++;

	// Increment the history position
	oxy_stringinput.history_pos = oxy_stringinput.history_size;
}

void oxy_Message(const char *title, char *message)
{
	const int width = 100;
	const int height = 80;
	const uint8_t titlewidth = 12;
	const uint8_t padding = 3;
	const uint8_t numlines = 7;

	uint8_t xprint = 160 - (100 + 2 * padding) / 2;
	uint8_t yprint;

	if (title)
	{
		yprint = 120 - (height + 13) / 2;
		// oxy_CenteredWindow(title, width, height);
		oxy_PrintWordWrap(message, xprint + 2 * padding, yprint + titlewidth + 1, width, numlines, 0);
	}
	else
	{
		yprint = 120 - (numlines * 3 + titlewidth + 2 * padding) / 2;
		oxy_FillRoundRectangle(xprint, yprint, width, height, 0);
		oxy_PrintWordWrap(message, xprint + 2 * padding, yprint + titlewidth + padding + 1, width, numlines, 0);
	}
}

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
	// }

	static void oxy_UpdateString(void)
	{
		const char *keys[3] = {"\0\0\0\0\0\0\0\0\0\0\0WRMH\0\0?\0VQLG\0\0.ZUPKFC\0 YTOJEB\0\0XSNIDA\0\0\0\0\0\0\0\0\0",
							   "\0\0\0\0\0\0\0\0\0\0\0wrmh\0\0?\0vqlg\0\0.zupkfc\0 ytojeb\0\0xsnida\0\0\0\0\0\0\0\0\0",
							   "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0-\x33\x36\x39)\0\0\0.\x32\x35\x38(\0\0\0\x30\x31\x34\x37,\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"};
		uint8_t skKey;

		skKey = os_GetCSC();

		// Function Check while looping
		if (oxy_stringinput.enable_function_check)
		{
			oxy_stringinput.function_check_result = oxy_stringinput.function_check(NULL);
		}

		// No key was pressed return.
		if (!skKey)
			return;

		// Input text based on keys array
		if (keys[oxy_stringinput.type][skKey] && oxy_stringinput.charsamount - 1 != oxy_stringinput.max_char)
		{
			oxy_stringinput.charsamount++;
			oxy_stringinput.text[oxy_stringinput.charsamount - 2] = keys[oxy_stringinput.type][skKey];
			// 		// 	}

			//
			if (oxy_stringinput.type == 2 && oxy_stringinput.charsamount - 1 != oxy_stringinput.max_char)
			{
				char *append_str = NULL;

				switch (skKey)
				{
				case sk_Add:
					append_str = "+";
					break;

				case sk_Sub:
					append_str = "-";
					break;

				case sk_Div:
					append_str = "/";
					break;

				case sk_Mul:
					append_str = "*";
					break;

				case sk_Comma:
					append_str = ",";
					break;

				case sk_LParen:
					append_str = "(";
					break;

				case sk_RParen:
					append_str = ")";
					break;

				case sk_Sin:
					append_str = "Sin(";
					break;
				case sk_Cos:
					append_str = "Cos(";
					break;
				case sk_Tan:
					append_str = "Tan(";
					break;
				case sk_Recip:
					append_str = "^-1";
					break;
				case sk_Square:
					append_str = "^2";
					break;
				case sk_Log:
					append_str = "Log(";
					break;
				case sk_Ln:
					append_str = "Ln(";
					break;

				case sk_Power:
					append_str = "^";
					break;
				}

				if (append_str)
				{
					oxy_stringinput.charsamount += strlen(append_str);
					strcat(oxy_stringinput.text, append_str);
				}
			}

			// Delete the previous element
			if (skKey == sk_Del && oxy_stringinput.charsamount > 1)
			{
				oxy_stringinput.charsamount--;
				oxy_stringinput.text[oxy_stringinput.charsamount - 1] = '\0';
				// 	}

				// Clear or exit text input
				if (skKey == sk_Clear)
				{
					if (oxy_stringinput.text[0] == '\0')
					{
						oxy_stringinput.forced_exit = 1;
					}
					else
						oxy_ResetStringInput();
				}

				// Autofill the text
				if (oxy_stringinput.enable_autofill)
				{
				}

				// Triverse the History
				if (oxy_stringinput.enable_history)
				{
					if (skKey == sk_Up)
					{
						if (oxy_stringinput.history_pos > 0)
						{
							oxy_stringinput.history_pos--;
							if (oxy_stringinput.history[oxy_stringinput.history_pos])
							{
								strcpy(oxy_stringinput.text, oxy_stringinput.history[oxy_stringinput.history_pos]);
								oxy_stringinput.charsamount = strlen(oxy_stringinput.text) + 1;
							}
						}
					}

					if (skKey == sk_Down)
					{
						if (oxy_stringinput.history_pos < oxy_stringinput.history_size - 1)
						{
							oxy_stringinput.history_pos++;
							if (oxy_stringinput.history[oxy_stringinput.history_pos])
							{
								strcpy(oxy_stringinput.text, oxy_stringinput.history[oxy_stringinput.history_pos]);
								oxy_stringinput.charsamount = strlen(oxy_stringinput.text) + 1;
							}
						}

						if (oxy_stringinput.history_pos + 1 == oxy_stringinput.history_size)
						{
							oxy_stringinput.history_pos = oxy_stringinput.history_size;
							oxy_ResetStringInput();
						}
					}
				}

				// Change the input text type
				if (skKey == sk_Alpha)
				{
					if (oxy_stringinput.type + 1 > 2)
					{
						oxy_stringinput.type = 0;
					}
					else
						oxy_stringinput.type++;

					// 	}

					oxy_stringinput.text[oxy_stringinput.charsamount - 1] = '\0';
				}

				char *oxy_StringInput(const char title[], uint16_t x, uint8_t y, uint8_t maxchar)
				{
					char temp[2] = "";
					uint8_t color = gfx_SetTextBGColor(0); // Get previous color
					oxy_stringinput.max_char = maxchar;

					// Prepare the string for input
					oxy_ResetStringInput();

					// Used for background color matching
					gfx_SetTextBGColor(color);
					gfx_SetColor(color); // Set background to current previous color

					kb_Scan();

					while (!(kb_Data[6] & kb_Enter) && !oxy_stringinput.forced_exit && !oxy_stringinput.function_check_result)
					{
						oxy_UpdateString();

						kb_Scan();

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
						gfx_FillRectangle(x + gfx_GetStringWidth(title), y, gfx_GetStringWidth(oxy_stringinput.text) + 10, 9); // Text buffer.

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

						gfx_FillRectangle(x + gfx_GetStringWidth(title), y, gfx_GetStringWidth(oxy_stringinput.text) + 10, 9); // Text buffer.
					}

					/* Removing Cursor */
					gfx_FillRectangle(x + gfx_GetStringWidth(title), y, gfx_GetStringWidth(oxy_stringinput.text) + gfx_GetStringWidth("|"), 9);
					gfx_Blit(1);

					return oxy_stringinput.text;
				}

				void oxy_Dialogue(const char *title, char *message, struct oxy_button_t **buttons)
				{
					const int width = 110;
					const int height = 90;
					const uint8_t titlewidth = 12;
					const uint8_t padding = 3;
					const uint8_t numlines = 7;

					uint8_t xprint = 160 - (100 + 2 * padding) / 2;
					uint8_t yprint;

					// Print with title
					if (title)
					{
						yprint = 120 - (height + 13) / 2;
						// oxy_CenteredWindow(title, width, height);
						oxy_PrintWordWrap(message, xprint + 2 * padding, yprint + titlewidth + 1, width, numlines, 0);
					}
					else
					{ // Print with no title
						yprint = 120 - (numlines * 3 + titlewidth + 2 * padding) / 2;
						oxy_FillRoundRectangle(xprint, yprint, width, height, 0);
						oxy_PrintWordWrap(message, xprint + 2 * padding, yprint + titlewidth + padding + 1, width, numlines, 0);
					}

					// Display the buttons here
				}

				uint8_t oxy_ColorPicker(uint8_t cur_select, uint16_t x, uint8_t y)
				{
					uint8_t color = cur_select;
					uint8_t count = 0;

					// Start double buffer
					gfx_sprite_t *back_buff = gfx_MallocSprite(8 * 64, 8 * 8);
					gfx_GetSprite(back_buff, x, y);

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

					gfx_Sprite(back_buff, x, y);
					free(back_buff);

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
					int bat_status = boot_GetBatteryStatus();

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
						{
							gfx_SetColor(0xE2);
						}

						bat_status = bat_status * 2.25;
					}
					else
					{
						gfx_SetColor(0xE0);
						bat_status = 1;
					}

					gfx_FillRectangle(x + 2, y + 2, bat_status, 6);
				}