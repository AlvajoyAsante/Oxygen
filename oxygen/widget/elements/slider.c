#include "slider.h"
#include "../color.h"
#include "../../oxy_gfx.h"

#include <keypadc.h>
#include <graphx.h>

void oxy_UpdateSlider(struct oxy_widget_t *widget)
{
    struct oxy_slider_t *slider = (struct oxy_slider_t *)widget;

    if (widget->state.visible && oxy_CheckCursorOverlap(widget))
    {
        /* Handle key press */
        bool left_pressed = kb_IsDown(kb_KeyAlpha) && kb_IsDown(kb_KeyLeft) ? true : false ;
        bool right_pressed = kb_IsDown(kb_KeyAlpha) && kb_IsDown(kb_KeyRight) ? true : false ;

        if (left_pressed || right_pressed)
        {
            if (left_pressed) {
                slider->curr_value--;
            }
                
            
            if (right_pressed) { 
                slider->curr_value++;
            }

            widget->state.redraw = true;
        }

        /* Handle out of bound value */
        if (slider->curr_value > slider->max)
        {
            slider->curr_value = slider->max;
        }

        if (slider->curr_value < slider->min)
        {
            slider->curr_value = slider->min;
        }
    }
}

void oxy_RenderSlider(struct oxy_widget_t *widget)
{
    struct oxy_slider_t *slider = (struct oxy_slider_t *)widget;

    /* Check if the slider is visible */
    if (widget->state.visible && widget->state.redraw)
    {
        /* Determine the width based on if the user wants to render value of slider*/
        uint16_t width = slider->render_value ? (widget->size.width - 18) - 1 : widget->size.width;

        /* Determine filled area */
        float filled = (float)(slider->curr_value - slider->min) / (slider->max - slider->min);

        /* Render the outline */
        oxy_OutlinedRectangle(widget->position.x, widget->position.y, width, widget->size.height, OXY_BUTTON_FILL_COLOR, OXY_BUTTON_BORDER_COLOR);

        /* Render the inner part */
        gfx_SetColor(OXY_BUTTON_BG_SELECTED_COLOR);
        gfx_FillRectangle(widget->position.x + 2, widget->position.y + 2, (width - 4) * filled, widget->size.height - 4);

        /* Render text value */
        if (slider->render_value)
        {
            gfx_PrintStringXY((char *)slider->curr_value, width + 1, widget->position.y);
        }
    }
}
