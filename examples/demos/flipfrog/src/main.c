// Keep these 
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <tice.h>

// Standard headers
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Graphic libs
#include <graphx.h>
#include <keypadc.h>

// my headers
#include "main.h"
#include "oxy/oxy_gui.h"
#include "oxy/oxy_mouse.h"
#include "gfx/frog_gfx.h"

int change_time = 1;
int return_animation;
struct frog_data_t frog_data;
struct menu_data_t menu_data;
// int timer_control = TIMER1_ENABLE | TIMER1_32K | TIMER_UP;

gfx_UninitedSprite(frog_buffer, 32, 32);

gfx_sprite_t *frog[19] = {frog_1, frog_2, frog_3, frog_4, frog_5, frog_6, frog_7, frog_8, frog_9, frog_10, frog_11,
							frog_12, frog_13, frog_14, frog_15, frog_16, frog_17, frog_18, frog_19};

void main(void)
{
	gfx_Begin();
	
	gfx_SetPalette(palette, sizeof_palette, 0);
	gfx_SetTransparentColor(255);
	
	gfx_SetDraw(1);
	
	oxy_InitMouse();
	vars_init();
	
	while (kb_Data[6] != kb_Clear){
		kb_Scan();
		UI_setup();
		DEBUG();
		
		if (menu_data.more_menu <= 1){
			frog_costume_draw();
		}else{
			more_menu_clicked();
		}
		
		gfx_SetTextBGColor(2);
		gfx_SetTextFGColor(1);
		oxy_DrawMouse_NoLoop();
		
		if (frog_data.timer != change_time){
			frog_data.timer++;
		}else if (frog_data.timer >= change_time){
			frog_data.timer = 0;
		}
		
		gfx_Blit(1);
		
		if (kb_Data[6] == kb_Enter || kb_Data[1] == kb_2nd){
			mouse_enter(oxy_HoverDetect());
		}
	}
	
	gfx_End();
	
	return;
}

void frog_costume(void) // manages the frogs costumes (just increased optimization)
{
	uint8_t max_costume, return_costume;
	bool brup_check = false;
	
	frog_data.y = (LCD_HEIGHT - (32*frog_data.size) )/2;
	
	switch (frog_data.animation) {
		case 1:
			frog_data.x = (LCD_WIDTH - (32*frog_data.size))/2;
			return_costume = 0;
			max_costume = 8;
			frog_data.rotated = 0;
			break;
			
		case 2:
			frog_data.x = (LCD_WIDTH - (32*frog_data.size))/2;
			return_costume = 9;
			max_costume = 11;
			
			if (randInt(0,1)){
				frog_data.rotated = 1;
			}else{
				frog_data.rotated = 0;
			}
			break;
			
		case 3:
			return_costume = 12;
			max_costume = 15;
			
			if (frog_data.size > 1) { // just in-case size is way too big
				frog_data.x = (LCD_WIDTH - (32*frog_data.size))/2;
				break;
			}
			
			if (!frog_data.rotated) {
				if (frog_data.x <= 10) {
					frog_data.rotated = 1;
					frog_data.x += 3;
				}else{
					frog_data.rotated = 0;
					if (frog_data.costume != 12) frog_data.x -= 3;
				}
			}else{
				if (frog_data.x >= 288) {
					frog_data.rotated = 0;
				}else{
					frog_data.rotated = 1;
					if (frog_data.costume != 12) frog_data.x += 3;
				}
			}
			break;
			
		case 4:
			return_costume = 16;
			max_costume = 18;
			
			if (frog_data.size > 1) { // just in-case size is way too big
				frog_data.x = (LCD_WIDTH - (32*frog_data.size))/2;
				break;
			}
			
			if (!frog_data.rotated) { 
				if (frog_data.x <= 10) frog_data.rotated = 1;
			}else{
				if (frog_data.x >= LCD_WIDTH - 32) frog_data.rotated = 0;
			}
			break;
	}
	

	// Animation Loop
	if (frog_data.costume != max_costume) {
		
		if (frog_data.timer >= change_time) {
			
			if (!frog_data.ret_animation) {
				
				if (!(frog_data.costume == 12 || frog_data.costume == 16)) {
					
					frog_data.costume++;
					brup_check = false;
					
				} else {
					
					switch (frog_data.costume) {
						case 12:
							if (randInt(0,1) || brup_check == true) {
								if (frog_data.timer >= change_time + 1) frog_data.costume++;
							} else {
								frog_data.costume = 16;
								frog_data.animation = 4;
							}
							break;
						
						case 16:
							if (randInt(0,1)) {
								frog_data.costume++;
							} else {
								frog_data.costume = 13;
								frog_data.animation = 3;
								brup_check = true;
							}
							break;
					}
					
				}
				
			} else {
				
				frog_data.costume--;
				
			}
			
		}
		
	} else {
		
		if (frog_data.timer >= change_time)
			frog_data.costume = return_costume;
		
		
	}
	
	return; 
}

void frog_costume_draw(void)
{	
	frog_costume();
	
	if (!frog_data.rotated) {
		
		gfx_ScaledTransparentSprite_NoClip(frog[frog_data.costume], frog_data.x, frog_data.y, frog_data.size, frog_data.size);
		
	}else gfx_ScaledTransparentSprite_NoClip(gfx_FlipSpriteY(frog[frog_data.costume], frog_buffer), frog_data.x, frog_data.y, frog_data.size, frog_data.size);
	return;
}

void UI_setup(void)
{
	gfx_FillScreen(frog_data.color[2]);
	
	gfx_SetColor(2);
	
	oxy_RoundFillRectangle(6, 225, 54, 14);
	oxy_RoundFillRectangle(260, 225, 54, 14);
	
	gfx_SetTextFGColor(1);
	gfx_SetTextBGColor(2);
	
	gfx_SetTextXY((56 - (gfx_GetStringWidth("CHANGE")))+ 2, 229);
	gfx_PrintString("CHANGE");
	
	gfx_SetTextXY((315 - (gfx_GetStringWidth("MORE"))) - 11, 229);
	gfx_PrintString("MORE");
	
	if (menu_data.change_menu >= 1){
		oxy_RoundFillRectangle(6, 208, 54, 14);
		gfx_SetTextXY((56 - (gfx_GetStringWidth("COLOR"))) - 2, 212);
		gfx_PrintString("COLOR");
		
		oxy_RoundFillRectangle(6, 193, 54, 14);
		gfx_SetTextXY((56 - (gfx_GetStringWidth("SIZE"))) - 7, 197);
		gfx_PrintString("SIZE");
		
		oxy_RoundFillRectangle(6, 178, 54, 14);
		gfx_SetTextXY((56 - (gfx_GetStringWidth("ANIMAT")))+ 1, 182);
		gfx_PrintString("ANIMAT");
	}
	
	switch (menu_data.change_menu){
		case 2: // COLOR
			gfx_SetColor(2);
			oxy_RoundRectangle(63, 208, 14, 14);
			oxy_RoundRectangle(78, 208, 14, 14);
			oxy_RoundRectangle(93, 208, 14, 14);
			
			gfx_SetColor(frog_data.color[0]);
			gfx_FillRectangle(64, 209, 12, 12);
			gfx_SetColor(frog_data.color[1]);
			gfx_FillRectangle(79, 209, 12, 12);
			gfx_SetColor(frog_data.color[2]);
			gfx_FillRectangle(94, 209, 12, 12);
			break;
			
		case 3: // SIZE
			oxy_RoundFillRectangle(63, 193, 28, 14);
			gfx_SetTextXY((89 - (gfx_GetStringWidth("+1"))), 197);
			gfx_PrintString("+1");
			
			oxy_RoundFillRectangle(92, 193, 28, 14);
			gfx_SetTextXY((117 - (gfx_GetStringWidth("-1"))), 197);
			gfx_PrintString("-1");
			break;
			
		case 4: // ANIMATION
			oxy_RoundFillRectangle(63, 178, 14, 14);
			gfx_SetTextXY((78 - (gfx_GetStringWidth("1"))) - 3, 182);
			gfx_PrintString("1");
			
			oxy_RoundFillRectangle(78, 178, 14, 14);
			gfx_SetTextXY((91 - (gfx_GetStringWidth("2"))) - 1, 182);
			gfx_PrintString("2");
			
			oxy_RoundFillRectangle(93, 178, 14, 14);
			gfx_SetTextXY((106 - (gfx_GetStringWidth("3"))) - 1, 182);
			gfx_PrintString("3");
			break;		
	}
	
	if (menu_data.more_menu >= 1){
		oxy_RoundFillRectangle(260, 208, 54, 14);
		gfx_SetTextXY((315 - (gfx_GetStringWidth("CREDIT")) - 4), 212);
		gfx_PrintString("CREDIT");
		
		oxy_RoundFillRectangle(260, 193, 54, 14);
		gfx_SetTextXY((315 - (gfx_GetStringWidth("ABOUT")) - 10), 197);
		gfx_PrintString("ABOUT");
	}
	
	gfx_SetTextFGColor(2);
	gfx_SetTextBGColor(frog_data.color[2]);
	gfx_SetTextXY((LCD_WIDTH - gfx_GetStringWidth(FROG_VER))/2, 229);
	gfx_PrintString(FROG_VER);
	return;
}

void more_menu_clicked(void)
{
	gfx_SetTextFGColor(1);
	gfx_SetTextBGColor(frog_data.color[2]);
	
	switch (menu_data.more_menu){
		case 2: // CREDITS
			gfx_SetTextXY((LCD_WIDTH - gfx_GetStringWidth(DEV_INFO)) / 2, (LCD_HEIGHT - 10) / 2);
			gfx_PrintString(DEV_INFO);
			break;
			
		case 3:
			gfx_SetTextXY((LCD_WIDTH - gfx_GetStringWidth("FLIPFROG USES XENON NEW LIBARY (OXYGEN)")) / 2, LCD_HEIGHT/2);
			gfx_PrintString("FLIPFROG USES XENON NEW LIBARY (OXYGEN)");
			
			gfx_SetTextXY((LCD_WIDTH - gfx_GetStringWidth("WITH LOADS OF MORE FEATURES COMING SOON!")) / 2, (LCD_HEIGHT/2) + 10);
			gfx_PrintString("WITH LOADS OF MORE FEATURES COMING SOON!");
			break;
	}
	
	return;
}

void mouse_enter(int index) // developers make there own enter functions with oxygen
{
	switch (index){
		case 0:
			if (!menu_data.change_menu){
				if (!menu_data.more_menu){
					menu_data.change_menu++;
				}else{
					menu_data.more_menu = 0;
					menu_data.change_menu++;
				}
				active_hover_index(index);
				remove_hover_index(1);
			}else{
				menu_data.change_menu = 0;
				remove_hover_index(index);
				remove_hover_index(1);
			}
			delay(100);
			break;
		
		case 1: // colors main but.
			menu_data.change_menu = 2;
			active_hover_index(index);
			break;
		
		case 2: // size main but.
			menu_data.change_menu = 3;
			active_hover_index(index);
			break;
		
		case 3: // animation main but.
			menu_data.change_menu = 4;
			active_hover_index(index);
			break;
		
		case 4:
		
			switch (menu_data.change_menu){
				case 2: // color button (skin)
					frog_data.color[0] = change_frog_colors(frog_data.color[0]);
					break;
					
				case 3:
					if (frog_data.size <= 5) frog_data.size++;
					break;
					
				case 4:
					frog_data.animation = 1;
					frog_data.costume = 1;
					break;
			}
			
			break;
		
		case 5: 
		
			switch (menu_data.change_menu){
				case 2: // color button
					frog_data.color[1] = change_frog_colors(frog_data.color[1]);
					break;
					
				case 3:
					if (frog_data.size > 1) frog_data.size--;
					break;
					
				case 4:
					frog_data.costume = 9;
					frog_data.animation = 2;
					break;
			}
			
			break;
			
		case 6:
			
			switch (menu_data.change_menu){
				case 2: // color button
					frog_data.color[2] = oxy_ColorPicker(frog_data.color[2], 1, 1);
					break;
					
				case 4:
					frog_data.costume = 12;
					frog_data.animation = 3;
					frog_data.x = LCD_WIDTH - (32*frog_data.size);
					break;
			}
			
			break;
		
		case 10: // more main but.
			if (!menu_data.more_menu){
				if (!menu_data.change_menu){
					menu_data.more_menu++;
				}else{
					menu_data.change_menu = 0;
					menu_data.more_menu++;
				}
				active_hover_index(index);
			}else{
				menu_data.more_menu = 0;
				remove_hover_index(index);
			}
			delay(100);
			break;
			
		case 11:
			menu_data.more_menu = 2;
			break;
			
		case 12:
			menu_data.more_menu = 3;
			break;
	}
	return;
}

uint8_t change_frog_colors(const uint8_t curr_color) // ported to oxygen
{
	const uint8_t new_color = oxy_ColorPicker(curr_color, 1, 1); 
	
	if (!new_color) return 0;
	
	for (int i = 0; i < 20; i++){
		uint8_t *data = frog[i]->data;
		uint8_t width = frog[i]->width;
		uint8_t height = frog[i]->height;
		
		for (int j = 0; j < width * height; j++)
		{
			if (data[j] == curr_color)
				data[j] = new_color;
		}
	}
	
	return new_color;
}

void active_hover_index(int index)
{
	switch(index){
		case 0: // change menu
			remove_hover_index(10);
			oxy_AddHover(6, 208, 54, 14, 1);
			oxy_AddHover(6, 193, 54, 14, 2);
			oxy_AddHover(6, 178, 54, 14, 3);
			oxy_SetHoverDescription("CHANGE COLOR", 1);
			oxy_SetHoverDescription("CHANGE SIZE", 2);
			oxy_SetHoverDescription("CHANGE ANIMATION", 3);
			break;
			
		case 1: // colors
			remove_hover_index(1);
			oxy_AddHover(63, 208, 14, 14, 4);
			oxy_AddHover(78, 208, 14, 14, 5);
			oxy_AddHover(93, 208, 14, 14, 6);
			oxy_SetHoverDescription("SKIN COLOR", 4);
			oxy_SetHoverDescription("BELLY COLOR", 5);
			oxy_SetHoverDescription("BACKGOUND COLOR", 6);
			break;
			
		case 2: // size
			remove_hover_index(1);
			oxy_AddHover(63, 193, 28, 14, 4);
			oxy_AddHover(92, 193, 28, 14, 5);
			oxy_SetHoverDescription("INCREASE", 4);
			oxy_SetHoverDescription("DECREASE", 5);
			break;
		
		case 3: // animation type
			remove_hover_index(1);
			oxy_AddHover(63, 178, 14, 14, 4);
			oxy_AddHover(78, 178, 14, 14, 5);
			oxy_AddHover(93, 178, 14, 14, 6);
			oxy_SetHoverDescription("DANCE", 4);
			oxy_SetHoverDescription("BLINK", 5);
			oxy_SetHoverDescription("JUMP", 6);
			break;
			
		case 10:
			remove_hover_index(0);
			oxy_AddHover(260, 208, 54, 14, 11);
			oxy_AddHover(260, 193, 54, 14, 12);
			oxy_SetHoverDescription("DEV CREDIT", 11);
			oxy_SetHoverDescription("DEV INFORMATION", 12);
			break;
	}
	return;
}

void remove_hover_index(int index)
{
	switch(index){
		case 0:
			oxy_RemoveHover(1);
			oxy_RemoveHover(2);
			oxy_RemoveHover(3);
			break;
			
		case 1:
			oxy_RemoveHover(4);
			oxy_RemoveHover(5);
			oxy_RemoveHover(6);
			break;
		
		case 10:
			oxy_RemoveHover(11);
			oxy_RemoveHover(12);
			break;
	}	
	return;
}

// routines below!!
void vars_init(void)
{
	oxy_AddHover(6, 225, 54, 14, 0);
	oxy_AddHover(260, 225, 54, 14, 10);
	oxy_AddHover(142, 225, 35, 14, 2);
	
	oxy_SetHoverDescription("CHANGE ASPECTS", 0);
	oxy_SetHoverDescription("MORE INFORMATION", 10);
	oxy_SetHoverDescription("DEMO VERSION", 2);
	
	menu_data.change_menu = frog_data.costume = frog_data.timer = menu_data.more_menu = 0;
	frog_data.size = frog_data.animation = 1;
	frog_data.color[0] = 4;
	frog_data.color[1] = 5;
	frog_data.color[2] = 3;
	
	timer_Disable(1);
	timer_Set(1, ONE_SECOND);
    timer_SetReload(1, ONE_SECOND);
	timer_Enable(1, TIMER_32K, TIMER_NOINT, TIMER_UP);
	
	return;
}

void DEBUG(void)
{
	if (DEV_MODE) {
		// so your looking at my code huh!?
		gfx_SetTextXY(1, 1);
		gfx_PrintString("x: ");
		gfx_PrintUInt(frog_data.x, 2);
		gfx_PrintString(", ");
		gfx_PrintString("y: ");
		gfx_PrintUInt(frog_data.y, 2);
		
		gfx_SetTextXY(1, 10);
		gfx_PrintUInt(frog_data.costume, 2); // current costume #
		gfx_PrintString(", ");
		gfx_PrintUInt(frog_data.animation, 2); // animation type #
		gfx_PrintString(", ");
		gfx_PrintUInt(frog_data.rotated, 2); // is object rotated
	}else{
		gfx_SetTextXY(1, 1);
		gfx_PrintUInt((uint8_t)TIMER_FREQ/timer_Get(1), 2);
		timer_Set(1, 0);
	}
	
	return;
}

