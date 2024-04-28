/*
 *--------------------------------------
 * Program Name: Demo
 * Author: Alvajoy 'Alvajoy123' Asante
 * Description: Oxygen Build Demo
 *--------------------------------------
 */

#include <tice.h>
#include <graphx.h>
#include <keypadc.h>

#include "oxygen/oxygen.h"

int main(void)
{
    /* Create a button */
    struct oxy_widget_t button_widget = {
        .position = {
            .x = 10,
            .y = 10,
        },

        .size = {
            .width = 30,
            .height = 30,
        },

        .child = NULL};

    /* Start The Graphics */
    gfx_Begin();

    /* Initialize the Widgets */
    oxy_InitWidget(&button_widget, OXY_BUTTON_TYPE);
    // oxy_UpdateButton(&button_widget);
    // oxy_RenderButton(&button_widget);
    button_widget.update(&button_widget);
    button_widget.render(&button_widget);

    /* Pause and wait for user input */
    while (!os_GetCSC());   

    /* End Graphics */
    gfx_End();

    return 0;
}
