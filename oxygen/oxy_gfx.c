#include "oxy_gfx.h"
#include "gfx/oxy_sprites.h"

#include <graphx.h>
#include <fileioc.h>
#include <string.h>
#include <math.h>
#include <debug.h>
#include <ctype.h>

// Shapes Routines
void oxy_CheckeredRectangle(uint16_t x, uint8_t y, int w, uint8_t h)
{
	for (int row = 0; row < h; row++)
	{
		for (int col = 0; col < w; col++)
		{
			if (!(col % 2))
			{
				if (!(row % 2))
				{
					gfx_SetPixel(x + row, y + col);
				}
			}
			else
			{
				if (row % 2)
				{
					gfx_SetPixel(x + row, y + col);
				}
			}
		}
	}
}

void oxy_FillRoundRectangle(uint16_t x, uint8_t y, int w, uint8_t h, uint8_t r)
{
	if (r >= h / 2 || r >= w / 2)
	{
		gfx_FillCircle_NoClip(x + w / 2, y + h / 2, r);
		return;
	}

	// Fill the four corners of the rectangle with circles
	gfx_FillCircle_NoClip(x + r, y + r, r);
	gfx_FillCircle_NoClip(x + r, y + h - r, r);
	gfx_FillCircle_NoClip(x + w - r, y + r, r);
	gfx_FillCircle_NoClip(x + w - r, y + h - r, r);

	// Fill the remaining parts of the rectangle with rectangles
	gfx_FillRectangle_NoClip(x + r, y, w - r * 2, h + 1);
	gfx_FillRectangle_NoClip(x, y + r, w + 1, h - r * 2);
}

void oxy_RoundRectangle(int16_t x, int8_t y, int w, uint8_t h, uint8_t r)
{
	int16_t x_min, x_max, y_min, y_max;
	int16_t a, b, P;

	x_min = x + r;
	y_min = y + r;
	x_max = x + w - r;
	y_max = y + h - r;

	a = r;
	b = 0;
	P = 1 - a;

	while (a >= b)
	{
		// Top Left
		gfx_SetPixel(x_min - a, y_min - b);
		gfx_SetPixel(x_min - b, y_min - a);

		// Top Right
		gfx_SetPixel(x_max + a, y_min - b);
		gfx_SetPixel(x_max + b, y_min - a);

		// Bottom Left
		gfx_SetPixel(x_min - a, y_max + b);
		gfx_SetPixel(x_min - b, y_max + a);

		// Bottom Right
		gfx_SetPixel(x_max + a, y_max + b);
		gfx_SetPixel(x_max + b, y_max + a);

		if (P < 0)
		{
			P += 3 + 2 * b;
		}
		else
		{
			P += 5 + 2 * (b - a);
			a--;
		}
		b++;
	}

	// Top and Bottom
	gfx_Line(x_min, y_min - r, x_max, y_min - r);
	gfx_Line(x_min, y_max + r, x_max, y_max + r);

	// Left and Right
	gfx_Line(x_min - r, y_min, x_min - r, y_max);
	gfx_Line(x_max + r, y_min, x_max + r, y_max);
}

void oxy_OutlinedRectangle(uint16_t x, uint8_t y, int w, uint8_t h, uint8_t fill_color, uint8_t outline_color)
{
	// Render the filled rectangle
	gfx_SetColor(fill_color);
	gfx_FillRectangle(x, y, w, h);

	// Render the outline rectangle
	gfx_SetColor(outline_color);
	gfx_Rectangle(x, y, w, h);
}

void oxy_OutlinedRoundRectangle(uint16_t x, uint8_t y, int w, uint8_t h, uint8_t type, uint8_t fill_color, uint8_t outline_color)
{
	// Render the filled rectangle
	gfx_SetColor(fill_color);
	oxy_FillRoundRectangle(x, y, w, h, type);

	// Render the outline rectangle
	gfx_SetColor(outline_color);
	oxy_RoundRectangle(x, y, w, h, type);
}

void oxy_FillIndentedRectangle(uint16_t x, uint8_t y, int w, uint8_t h, uint8_t type)
{
	switch (type)
	{
	case 0: // oxy_RoundFillRectangle
		for (int i = 0; i < w; i++)
		{
			if (i == 0 || i == w - 1)
			{
				gfx_VertLine(x + i, y + 1, h - 2);
			}
			else
				gfx_VertLine(x + i, y, h);
		}
		break;

	case 1: // oxy_RoundFillRectangle_Upper
		for (int i = 0; i < w; i++)
		{
			if (i == 0 || i == w - 1)
			{
				gfx_VertLine(x + i, y + 1, h - 1);
			}
			else
				gfx_VertLine(x + i, y, h);
		}
		break;

	case 2: // oxy_RoundFillRectangle_Bottom
		for (int i = 0; i < w; i++)
		{
			if (i == 0 || i == w - 1)
			{
				gfx_VertLine(x + i, y, h - 1);
			}
			else
				gfx_VertLine(x + i, y, h);
		}
		break;

	case 3: // oxy_RoundFillRectangle_Left
		for (int i = 0; i < w; i++)
		{
			if (i == 0)
			{
				gfx_VertLine(x + i, y + 1, h - 2);
			}
			else
				gfx_VertLine(x + i, y, h);
		}
		break;

	case 4: // oxy_RoundFillRectangle_Right
		for (int i = 0; i < w; i++)
		{
			if (i == w - 1)
			{
				gfx_VertLine(x + i, y + 1, h - 2);
			}
			else
				gfx_VertLine(x + i, y, h);
		}
		break;
	}
}

void oxy_IndentedRectangle(uint16_t x, uint8_t y, int w, uint8_t h, uint8_t type)
{
	switch (type)
	{
	case 0: // oxy_RoundRectangle
		gfx_HorizLine(x + 1, y, w - 2);
		gfx_HorizLine(x + 1, (y + h) - 1, w - 2);
		gfx_VertLine(x, y + 1, h - 2);
		gfx_VertLine((x + w) - 1, y + 1, h - 2);
		break;

	case 1: // oxy_RoundRectangle_Upper
		gfx_HorizLine(x + 1, y, w - 2);
		gfx_HorizLine(x, (y + h) - 1, w);
		gfx_VertLine(x, y + 1, h - 1);
		gfx_VertLine((x + w) - 1, y + 1, h - 1);
		break;

	case 2: // oxy_RoundRectangle_Bottom
		gfx_HorizLine(x, y, w);
		gfx_HorizLine(x + 1, (y + h) - 1, w - 2);
		gfx_VertLine(x, y, h - 1);
		gfx_VertLine((x + w) - 1, y, h - 1);
		break;

	case 3: // oxy_RoundRectangle_Left
		gfx_HorizLine(x + 1, y, w - 1);
		gfx_HorizLine(x + 1, (y + h) - 1, w - 1);
		gfx_VertLine(x, y + 1, h - 2);
		gfx_VertLine((x + w) - 1, y, h);
		break;

	case 4: // oxy_RoundRectangle_Right
		gfx_HorizLine(x, y, w);
		gfx_HorizLine(x, (y + h) - 1, w - 1);
		gfx_VertLine(x, y, h - 1);
		gfx_VertLine((x + w) - 1, y + 1, h - 2);
		break;
	}
}

void oxy_Curve(int x1, int y1, int x2, int y2, int x3, int y3)
{
	// calculate the number of steps to take along the curve
	const int steps = (abs(x2 - x1) + abs(x3 - x2)) * 4;
	double t = 0;

	// iterate through each step and calculate the x and y positions
	for (int i = 0; i < steps; i++)
	{
		int x = (1 - 2 * t + t * t) * x1 + 2 * (t - t * t) * x2 + t * t * x3;
		int y = (1 - 2 * t + t * t) * y1 + 2 * (t - t * t) * y2 + t * t * y3;

		// draw the pixel at the calculated position
		gfx_SetPixel(x, y);

		t += (1.0 / steps);
	}
}

void oxy_Arc(uint16_t x, uint8_t y, uint24_t radius, int start_angle, int end_angle)
{
	int xc = x, yc = y;
	int d = 1 - radius;
	int de = 3;
	int dse = -2 * radius + 5;

	x = 0;
	y = radius;

	// only draw pixels within the desired angle range
	while (y > x)
	{
		if (start_angle <= 45 && end_angle >= 45)
		{
			gfx_SetPixel(xc + x, yc + y);
		}
		if (start_angle <= 90 && end_angle >= 90)
		{
			gfx_SetPixel(xc + y, yc + x);
		}
		if (start_angle <= 135 && end_angle >= 135)
		{
			gfx_SetPixel(xc - x, yc + y);
		}
		if (start_angle <= 180 && end_angle >= 180)
		{
			gfx_SetPixel(xc - y, yc + x);
		}
		if (start_angle <= 225 && end_angle >= 225)
		{
			gfx_SetPixel(xc - y, yc - x);
		}
		if (start_angle <= 270 && end_angle >= 270)
		{
			gfx_SetPixel(xc - x, yc - y);
		}
		if (start_angle <= 315 && end_angle >= 315)
		{
			gfx_SetPixel(xc + x, yc - y);
		}
		if (start_angle <= 360 && end_angle >= 360)
		{
			gfx_SetPixel(xc + y, yc - x);
		}

		if (d < 0)
		{
			d += de;
			de += 2;
			dse += 2;
		}
		else
		{
			d += dse;
			de += 2;
			dse += 4;
			y--;
		}

		x++;
	}
}

// Sprite Routines.
/**
 * This function is use to replace the color in a sprite.
 */
gfx_sprite_t *oxy_ReplaceSpriteColor(gfx_sprite_t *in, const uint8_t old_color, const uint8_t new_color)
{
	int size = in->width * in->height;
	gfx_sprite_t *out;

	out = gfx_MallocSprite(in->width, in->height);
	memcpy(out, in, size + 2);

	for (int i = 0; i < size; i++)
	{
		if (out->data[i] == old_color)
			out->data[i] = new_color;
	}

	return out;
}

/**
 * This function cuts out sprite based on based on given mask.
 */
gfx_sprite_t *oxy_ApplyMaskToSprite(gfx_sprite_t *in, gfx_sprite_t *mask, const uint8_t mask_color, const uint8_t transparent_color)
{
	gfx_sprite_t *out, *resize;
	const int mask_size = mask->width * mask->height;
	const int in_size = in->width * in->height;
	const bool resized = (mask_size != in_size);

	out = gfx_MallocSprite(in->width, in->height);

	if (resized)
	{
		resize = gfx_MallocSprite(in->width, in->height);
		gfx_ScaleSprite(resize, mask);
	}

	for (int i = 0; i < in_size; i++)
	{
		if (!resized)
		{
			if (mask->data[i] == mask_color)
				out->data[i] = transparent_color;
		}
		else
		{
			if (resize->data[i] == mask_color)
				out->data[i] = transparent_color;
		}
	}

	return out;
}

/**
 * This function replaces the whole sprites palette.
 */
gfx_sprite_t *oxy_RepalettizeSprite(gfx_sprite_t *in, const uint16_t *palette, const uint8_t palette_size)
{
	gfx_sprite_t *out;
	const int size = in->width * in->height;
	out = gfx_MallocSprite(in->width, in->height);

	for (int i = 0; i < size; i++)
	{
		out->data[i] = oxy_Rgb1555toPalette((uint16_t)in->data[i], palette, palette_size);
	}

	return out;
}

// Color Routines.
/**
 * Condense Sprite into using a condensed version of Xlibc Palette
 * Created by Epsilon5!
 */
void oxy_CondenseSprite(gfx_sprite_t *in)
{
	for (int i = 0; i < in->width * in->height; i++)
		in->data[i] = xlibc_condensed_data[in->data[i]];
}

/**
 * Inserts a condensed version of Xlibc into the Palette
 */
void oxy_InsertCondensePalette(uint8_t offset)
{
	gfx_SetPalette(xlibc_condensed_palette, sizeof_xlibc_condensed_palette, offset);
}

/**
 * This function gets a list of colors in a sprite (not order from least to greatest).
 */
uint16_t *oxy_GetSpritePalette(gfx_sprite_t *in)
{
	const int size = in->width * in->height;
	uint16_t *output = (uint16_t *)malloc(sizeof(uint16_t) * size);
	uint8_t output_size = 0;

	for (int i = 0; i < size; i++)
	{
		for (int r = 0; r < output_size + 1; r++)
		{
			if (output[r] == in->data[i])
			{
				output[output_size] = in->data[i];
				output_size++;
			}
		}
	}

	return output;
}

/**
 * This function returns a palette color from a Rgb1555 color.
 */
uint8_t oxy_Rgb1555ToPalette(uint16_t color, const uint16_t *palette, const uint8_t palette_size)
{
	int Difference = 10000;
	uint8_t output;

	for (int i = 0; i < palette_size; i++)
	{
		if (oxy_ColorDifference(gfx_RGBTo1555(oxy_Rgb1555ToRGB(color)[0], oxy_Rgb1555ToRGB(color)[1], oxy_Rgb1555ToRGB(color)[2]), palette[i]) < Difference)
		{
			Difference = oxy_ColorDifference(gfx_RGBTo1555(oxy_Rgb1555ToRGB(color)[0], oxy_Rgb1555ToRGB(color)[1], oxy_Rgb1555ToRGB(color)[2]), palette[i]);
			output = i;
		}
	}

	return output;
}

/**
 * This function gives the closets color relating to the input Rgb1555
 */
uint8_t *oxy_Rgb1555ToRGB(uint16_t color)
{

	uint8_t *output = (uint8_t *)malloc(sizeof(uint8_t) * 3);

	output[0] = (uint8_t)(((color & 0xFC00) >> 10) << 3);
	output[1] = (uint8_t)(((color & 0x03E0) >> 5) << 3);
	output[2] = (uint8_t)((color & 0x001F) << 3);

	return output;
}

/**
 * This function returns the difference between two rgb1555 colors.
 */
int oxy_ColorDifference(uint16_t color1, uint16_t color2)
{
	return (abs(((int)(oxy_Rgb1555ToRGB(color1)[0])) - ((int)(oxy_Rgb1555ToRGB(color2)[0]))) + abs(((int)(oxy_Rgb1555ToRGB(color1)[1])) - ((int)(oxy_Rgb1555ToRGB(color2)[1]))) + abs(((int)(oxy_Rgb1555ToRGB(color1)[2])) - ((int)(oxy_Rgb1555ToRGB(color2)[2]))));
}

/**
 * This function returns a pointer to a list of darken palette colors.
 */
uint16_t *oxy_DarkenPalette(const uint8_t amount, uint16_t *palette, const uint8_t palette_size)
{
	uint16_t *output = (uint16_t *)malloc(sizeof(uint16_t) * palette_size);

	for (uint8_t i = 0; i < palette_size; i++)
	{
		output[i] = gfx_Darken(palette[i], amount);
	}

	return output;
}

/**
 * This function returns a pointer to a list of lighten palette colors.
 */
uint16_t *oxy_LightenPalette(const uint8_t amount, uint16_t *palette, const uint8_t palette_size)
{
	uint16_t *output = (uint16_t *)malloc(sizeof(uint16_t) * palette_size);

	for (uint8_t i = 0; i < palette_size; i++)
	{
		output[i] = gfx_Lighten(palette[i], amount);
	}

	return output;
}

/**
 * This function returns a the set transparent of "gfx" libary.
 * It's not needed but it's fun to have :).
 */
uint8_t oxy_ReturnTransparentColor(void)
{
	return gfx_SetTransparentColor(gfx_SetTransparentColor(gfx_SetTransparentColor(0)));
}

void oxy_SetTextColor(uint8_t fg, uint8_t bg)
{
	gfx_SetTextFGColor(fg);
	gfx_SetTextBGColor(bg);
	gfx_SetTextTransparentColor(bg);
}

/**
 * Implemented from "Captain-Calc/textioc"
 * This function works on the assumption that the longest word is <256 characters long
 */
static bool _iscntrl(int c)
{
	if (c == '\0' || c == '\n' || c == '\t')
	{
		return 1;
	}
	else
		return 0;
}

static uint8_t _getCharHeight(char *word)
{
	gfx_sprite_t *character;
	character = gfx_GetSpriteChar(*word);
	return character->height;
}

static uint8_t _getWordWidth(char *word)
{
	char *c = word;
	uint8_t width = 0;

	while (!isspace(*c) && !_iscntrl(*c) && width < 240)
		width += gfx_GetCharWidth(*c++);

	return width;
}

void oxy_PrintWordWrap(char *text, uint24_t x, uint8_t y, int width, uint8_t max_lines, uint8_t init_line)
{
	char *curr_char = text;
	uint24_t curr_line_width = 0;
	int word_width = 0;
	uint8_t curr_line_num = init_line;
	uint8_t i;
	uint8_t height = _getCharHeight(curr_char);
	uint8_t curr_line_print = 0;

	// Debugging
	// gfx_SetColor(224);
	// gfx_FillRectangle(x + width, 0, 2, LCD_HEIGHT);
	// gfx_FillRectangle(0, y, LCD_WIDTH, 2);

	if (gfx_GetStringWidth(text) <= (unsigned int)width)
	{
		gfx_SetTextXY(x, y);
		gfx_PrintString(text);
		return;
	};

	// If the width is way bigger than the LCD_Width
	if (width > LCD_WIDTH)
		width = LCD_WIDTH;

	for (;;)
	{

		if (curr_line_num > max_lines || y > LCD_HEIGHT)
			return;

		gfx_SetTextXY(x, y + (curr_line_num - init_line) * height);
		curr_line_width = 0;

		while (*curr_char != '\0')
		{

			// Get the width of the next word
			word_width = _getWordWidth(curr_char);

			// If there is room on the current line for the word, print the word
			// else, start a new line
			if (curr_line_width + word_width < (unsigned int)width)
			{
				curr_line_width += word_width;
				while (!isspace(*curr_char) && !iscntrl(*curr_char))
				{

					// Makes sure it's printing from correct line
					if (curr_line_print == curr_line_num)
					{
						gfx_PrintChar(*curr_char++);
					}
					else
					{
						curr_char++;
					}
				}
			}
			else
			{
				goto startLine;
			}

			if (*curr_char == '\0')
				return;

			switch (*curr_char++)
			{
			case '\t':
				if (curr_line_width + (4 * gfx_GetCharWidth(' ')) < (unsigned int)width)
				{
					for (i = 1; i < 4; i++)
					{
						gfx_PrintChar(' ');
						curr_line_width += gfx_GetCharWidth(' ');
					};
				}
				else
				{
					goto startLine;
				};
				break;

			case '\n':
				goto startLine;

			case '\v':
				goto startLine;

			case ' ':
				gfx_PrintChar(*(curr_char - 1));
				curr_line_width += gfx_GetCharWidth(' ');
				break;
			};
		};

	startLine:

		if (curr_line_print == curr_line_num)
		{
			curr_line_num++;
			curr_line_print++;
		}
		else
		{
			curr_line_print++;
		}
	};
}