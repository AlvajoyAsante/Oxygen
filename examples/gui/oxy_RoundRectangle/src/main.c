#include <tice.h>
#include <graphx.h>

#include "oxy/oxy_gui.h"

int main(void)
{

    /* Initialize graphics drawing */
    gfx_Begin();

    /* Draw to buffer instead of directly to screen */
    gfx_SetDraw(1);
	
	for (int c = 0; c < 5; c++){
		
		gfx_FillScreen(255);
		
		gfx_SetColor(c * 10);
		switch(c){
			case 0:
				oxy_RoundRectangle((LCD_WIDTH - 100)/2, (LCD_HEIGHT - 100)/2, 100, 100);
				break;
			case 1:
				oxy_RoundRectangle_Upper((LCD_WIDTH - 100)/2, (LCD_HEIGHT - 100)/2, 100, 100);
				break;
			case 2:
				oxy_RoundRectangle_Bottom((LCD_WIDTH - 100)/2, (LCD_HEIGHT - 100)/2, 100, 100);
				break;
			case 3:
				oxy_RoundRectangle_Left((LCD_WIDTH - 100)/2, (LCD_HEIGHT - 100)/2, 100, 100);
				break;
			case 4:
				oxy_RoundRectangle_Right((LCD_WIDTH - 100)/2, (LCD_HEIGHT - 100)/2, 100, 100);
				break;
		}
		
		gfx_Blit(1);
		
		/* Waits for a key */
		while (!os_GetCSC());
		
	}
    

    gfx_End();

    return 0;
}
