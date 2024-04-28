#include <tice.h>
#include <graphx.h>

#include "oxy/oxy_gui.h"

int main(void)
{
    /* Initialize graphics drawing */
    gfx_Begin();
	
	gfx_FillScreen(255);
	
	oxy_LibVersion(1, 1, 1);
	// while (!os_GetCSC());
	oxy_ResetStringInput();
	oxy_StringInput("DEMO: ", 18, 1, 10);
	
	// while (!os_GetCSC());
    
	gfx_End();

    return 0;
}
