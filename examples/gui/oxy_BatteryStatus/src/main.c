#include <tice.h>
#include <graphx.h>

#include "oxygen/oxygen.h"

int main(void)
{
    /* Initialize graphics drawing */
    gfx_Begin();
	gfx_FillScreen(255);
	
	/* Displays battery */
	oxy_BatteryStatus(1, 1);
	
	/* Waits for a key */
	while (!os_GetCSC());
    
	/* End graphics drawing */
	gfx_End();

    return 0;
}
