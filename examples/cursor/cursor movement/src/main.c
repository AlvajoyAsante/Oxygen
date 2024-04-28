#include <tice.h>
#include <graphx.h>
#include <keypadc.h>

#include "oxygen/oxygen.h"

int main(void)
{
    /* Initialize graphics drawing */
    gfx_Begin();
	gfx_FillScreen(255);
	
	/* Make sure you set this before running mouse commands */
	gfx_SetDraw(1);
	
	/* Initialize Mouse */
	oxy_InitMouse();
	
	/* Renders the Mouse */
	while (!(kb_Data[6] & kb_Clear)) {
		kb_Scan();
		oxy_RenderMouse();
	}
	
	/* End graphics drawing */
	gfx_End();

    return 0;
}