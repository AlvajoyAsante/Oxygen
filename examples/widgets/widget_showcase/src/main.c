#include <tice.h>
#include <graphx.h>
#include <keypadc.h>

#include "oxygen/oxygen.h"

static void button_clicked(void *arg)
{
    struct oxy_label_t *status = (struct oxy_label_t *)arg;
    if (status)
    {
        oxy_SetLabelText(&status->widget, "Button clicked");
    }
}

int main(void)
{
    struct oxy_label_t title_label = {0};
    struct oxy_label_t status_label = {0};
    struct oxy_label_t button_label = {0};
    struct oxy_button_t button = {0};
    struct oxy_checkbox_t checkbox = {0};
    struct oxy_switch_t switch_widget = {0};
    struct oxy_color_t theme = {0};

    struct oxy_widget_t *stack[] = {
        &button.widget,
        &checkbox.widget,
        &switch_widget.widget,
        &title_label.widget,
        &status_label.widget,
        &button_label.widget,
        NULL
    };

    gfx_Begin();
    gfx_SetDraw(1);
    gfx_FillScreen(255);

    oxy_InitCursor();
    oxy_InitMouse();
    oxy_InitConfig(stack, &theme);

    oxy_InitWidget(&title_label.widget, OXY_LABEL_TYPE);
    oxy_SetLabelText(&title_label.widget, "Widget showcase");
    title_label.widget.position.x = 8;
    title_label.widget.position.y = 6;
    title_label.widget.state.clickable = false;
    oxy_SetWidgetDescription(&title_label.widget, "A small widget demo");

    oxy_InitWidget(&status_label.widget, OXY_LABEL_TYPE);
    oxy_SetLabelText(&status_label.widget, "Try the widgets");
    status_label.widget.position.x = 8;
    status_label.widget.position.y = 18;
    status_label.widget.state.clickable = false;
    oxy_SetWidgetDescription(&status_label.widget, "Status output");

    oxy_InitWidget(&button_label.widget, OXY_LABEL_TYPE);
    oxy_SetLabelText(&button_label.widget, "Click me");
    button_label.widget.position.x = 18;
    button_label.widget.position.y = 38;
    button_label.widget.state.clickable = false;

    oxy_InitWidget(&button.widget, OXY_BUTTON_TYPE);
    button.widget.position.x = 10;
    button.widget.position.y = 34;
    button.widget.size.width = button_label.widget.size.width + 12;
    button.widget.size.height = 14;
    button.label = &button_label;
    oxy_SetWidgetRightClick(&button.widget, button_clicked, &status_label);
    oxy_SetWidgetDescription(&button.widget, "Primary click updates status");

    oxy_InitWidget(&checkbox.widget, OXY_CHECKBOX_TYPE);
    checkbox.widget.position.x = 10;
    checkbox.widget.position.y = 58;
    checkbox.widget.size.width = 10;
    checkbox.widget.size.height = 10;
    oxy_SetWidgetDescription(&checkbox.widget, "Toggle the checkbox");

    oxy_InitWidget(&switch_widget.widget, OXY_SWITCH_TYPE);
    switch_widget.widget.position.x = 10;
    switch_widget.widget.position.y = 74;
    switch_widget.widget.size.width = 16;
    switch_widget.widget.size.height = 8;
    oxy_SetWidgetDescription(&switch_widget.widget, "Toggle the switch");

    while (!os_GetCSC())
    {
        gfx_FillScreen(255);
        oxy_UpdateWidgets();
        oxy_RenderWidgets();
    }

    gfx_End();
    return 0;
}
