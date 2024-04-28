#include <tice.h>
#include <graphx.h>

#include "oxygen/oxygen.h"

int main(void)
{
    /* Initialize graphics drawing */
    gfx_Begin();
	gfx_FillScreen(255);
	
	/* Setting up displaying time */ 
	gfx_SetColor(255); // Include this before Running `oxy_GetTime`
	gfx_SetTextFGColor(0); // Text Color.
	oxy_GetTime(1, 1);
	
	/* Waits for a key */
	while (!os_GetCSC());
    
	/* End graphics drawing */
	gfx_End();

    return 0;
}
