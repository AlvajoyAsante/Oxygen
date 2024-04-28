#include <tice.h>
#include <graphx.h>
#include <keypadc.h>

#include "oxygen/oxygen.h"


static void oxy_FillRoundedRectangle(uint16_t x, uint8_t y, int w, uint8_t h, uint8_t r)
{
	gfx_FillCircle_NoClip(x + r, y + r, r);
	gfx_FillCircle_NoClip(x + w - r - 1, y + r, r);
	gfx_FillCircle_NoClip(x + r, y + h - r - 1, r);
	gfx_FillCircle_NoClip(x + w - r - 1, y + h - r - 1, r);
	gfx_FillRectangle_NoClip(x + r, y, w - r - r, h);
	gfx_FillRectangle_NoClip(x, y + r, w, h - r - r);
}

static void oxy_RoundedRectangle(uint16_t x, uint8_t y, int w, uint8_t h, uint8_t r)
{
	if (r > 7) r = 7;

	gfx_sprite_t *buff[3];

	// Allocate each index
	buff[0] = gfx_MallocSprite(w - 4, h - 4);	
	buff[1] = gfx_MallocSprite(w - (r*2) + 4, h - 2);	
	buff[2] = gfx_MallocSprite(w - 2, h - (r*2) + 4);	
	
	// Get the buffer sprites
	gfx_GetSprite(buff[0], x + 2, h + 2);
	gfx_GetSprite(buff[1], x + r - 2, y + 1);
	gfx_GetSprite(buff[2], x + 1, y + r - 2);
	
	// Print Round Rectanle
	oxy_FillRoundedRectangle(x, y, w, h, r);
	
	// Render and Free Sprites
	gfx_Sprite(buff[0], x + 2, y + 2);
	free(buff[0]);

	gfx_Sprite(buff[1], x + r - 2, y + 1);
	free(buff[1]);
	
	gfx_Sprite(buff[2], x + 1, y + r - 2);
	free(buff[2]);
}

int main(void)
{
    uint8_t index;
	
	/* Initialize graphics drawing */
	gfx_Begin();
	gfx_FillScreen(255);
	
	/* Make sure you set this before running mouse commands */
	gfx_SetDraw(1);
	
	/* Initialize Mouse */
	oxy_InitMouse();
	
	/* Initialize hover spot */
	gfx_SetColor(224);
	oxy_RoundedRectangle((LCD_WIDTH - 50)/2, (LCD_HEIGHT - 50)/2,50, 50, 15);
	index = oxy_AddHover((LCD_WIDTH - 50)/2, (LCD_HEIGHT - 50)/2, 50, 50); // Sets the hover spot to rectangle dimensions
	
	/* Initialize Description for the hover spot */
	gfx_SetTextFGColor(254);
	gfx_SetTextBGColor(0);
	oxy_SetHoverDescription("THIS IS A DEMO :)", index);
	
	/* Renders the Mouse */
	while (!(kb_Data[6] & kb_Clear)) {
		kb_Scan();
		oxy_RenderMouse();
	}
	
	/* End graphics drawing */
	gfx_End();

    return 0;
}
