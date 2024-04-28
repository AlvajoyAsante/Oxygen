#include <tice.h>
#include <graphx.h>
#include <keypadc.h>

#include "oxygen/oxygen.h"

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
	oxy_RoundRectangle((LCD_WIDTH - 50)/2, (LCD_HEIGHT - 50)/2,50, 50, 15, 0);
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
