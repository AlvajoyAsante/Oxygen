#include <tice.h>
#include <graphx.h>

#include "oxygen/oxygen.h"

int main(void)
{
	uint8_t color;
	
    /* Initialize graphics drawing */
    gfx_Begin();
	
	/* Displays Color Picker and stores color into 'Color' */
	color = oxy_ColorPicker(0, 1, 1);
	
	/* End graphics drawing */
    gfx_End();

    return 0;
}
