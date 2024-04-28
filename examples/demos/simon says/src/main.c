/*
 *--------------------------------------
 * Program Name: Simon Says
 * Author: Alvajoy123
 * License: MIT
 * Description: A simple game made using Oxygen.
 *--------------------------------------
*/

#include <tice.h>
#include <graphx.h>
#include <keypadc.h>
#include <debug.h>

#include "oxygen/oxy_mouse.h"
#include "oxygen/oxy_gfx.h"

#define MAX_SIMON_SAYS 10

/* Variables */
struct player_t {
	uint8_t score;
	uint8_t curr_step;
	uint8_t max_step;
	uint8_t wrong;
};
struct player_t player;

uint8_t simon_says[MAX_SIMON_SAYS]; // This is what Simon says, basically a list of what colors were said.

void render_click(void *data);


static void init_game(void) // Sets up variables
{
	oxy_InitMouse(); // make sure to include this 
	
	player.score = player.max_step = player.curr_step = player.wrong = 0;
	
	for (int i = 0; i < MAX_SIMON_SAYS; i++) { // This is what simon says
		simon_says[i] = randInt(0, 3);
	}
	
	// Set index colors (for boxes)
	gfx_palette[1] = gfx_RGBTo1555(155, 155, 0); // Dark Yellow 
	gfx_palette[2] = gfx_RGBTo1555(0, 0, 155); // Dark Blue
	gfx_palette[3] = gfx_RGBTo1555(155, 0, 0); // Dark Red
	gfx_palette[4] = gfx_RGBTo1555(0, 155, 0); // Dark Green
	
	// Draw the box
	gfx_SetColor(1);
	oxy_FillRoundRectangle(65, 26, 84, 84, 0); // Fully rounded rectangle
	oxy_AddHover(65, 26, 84, 84); // index 0
	oxy_detect[0].data = 0;
	oxy_SetRightClick(render_click, NULL, 0);
	
	gfx_SetColor(2);
	oxy_FillRoundRectangle(65, 129, 84, 84, 0);
	oxy_AddHover(65, 129, 84, 84); // index 1
	oxy_detect[1].data = 1; 
	oxy_SetRightClick(render_click, NULL, 1);
	
	gfx_SetColor(3);
	oxy_FillRoundRectangle(169, 26, 84, 84, 0);
	oxy_AddHover(169, 26, 84, 84); // index 2
	oxy_detect[2].data = 2;
	oxy_SetRightClick(render_click, NULL, 2);
	
	gfx_SetColor(4);
	oxy_FillRoundRectangle(169, 129, 84, 84, 0);
	oxy_AddHover(169, 129, 84, 84); // index 3
	oxy_detect[3].data = 3;
	oxy_SetRightClick(render_click, NULL, 3);
	
	gfx_SetColor(0);
	gfx_FillCircle(158, 119, 40);
	
	gfx_PrintStringXY("v0.1", (LCD_WIDTH - gfx_GetStringWidth("v0.1"))/2, 220);
	gfx_PrintStringXY("SIMON SAYS", (LCD_WIDTH - gfx_GetStringWidth("SIMON SAYS"))/2, 10);
	gfx_Blit(1);
}

static void render_says(uint8_t input) // Renders highlighted boxes
{
	// based on the given input the index color is changed and is updated during buffer.
	switch (input){
		case 0: // First Square
			gfx_palette[1] = gfx_RGBTo1555(255, 255, 0); // Yellow
			gfx_Blit(1);
			delay(150);
			gfx_palette[1] = gfx_RGBTo1555(155, 155, 0);
			break;
		
		case 1: // Second Square
			gfx_palette[2] = gfx_RGBTo1555(0, 0, 255); // Blue
			gfx_Blit(1);
			delay(150);
			gfx_palette[2] = gfx_RGBTo1555(0, 0, 155);
			break;
		
		case 2: // Third Square
			gfx_palette[3] = gfx_RGBTo1555(255, 0, 0); // Red
			gfx_Blit(1);
			delay(150);
			gfx_palette[3] = gfx_RGBTo1555(155, 0, 0);
			break;
		
		case 3: // Forth Square
			gfx_palette[4] = gfx_RGBTo1555(0, 255, 0); // Green
			gfx_Blit(1);
			delay(150);
			gfx_palette[4] = gfx_RGBTo1555(0, 155, 0);
			break;
	}
	gfx_Blit(1);
}

static void render_score(void)
{
	/* Displays the players score in the top left cornor. */
	gfx_SetColor(0);
	
	if (player.score < 10){
		gfx_FillRectangle((LCD_WIDTH - 8)/2, 118, 30, 10);
		gfx_SetTextXY((LCD_WIDTH - 8)/2, 118);
	}else{
		gfx_FillRectangle((LCD_WIDTH - 16)/2, 118, 30, 10);
		gfx_SetTextXY((LCD_WIDTH - 16)/2, 118);
	}
	
	gfx_PrintUInt(player.score, 1);
}

static void render_pattern(void)
{
	for (int i = 0; i < player.max_step + 1; i++){
		render_says(simon_says[i]);
		delay(100);
	}
}


int main(void)
{
	srand(rtc_Time());
	
	gfx_Begin();
	gfx_SetDrawBuffer(); // This is need for some oxygen functions to work
	gfx_FillScreen(0); // Fills the screen with black
	
	gfx_SetTextTransparentColor(254);
	gfx_SetTextFGColor(255);
	gfx_SetTextBGColor(0);
	
	init_game();
	render_score();
	render_says(simon_says[0]);
	gfx_Blit(1);
	delay(200);
	
	
	while (oxy_mouse.clicked_index != -3) {
		oxy_RenderMouse();
	}
	
	gfx_End();
	
    return 0;
}

void render_click(void *data) // global function for clicks
{
	uint8_t click = oxy_detect[oxy_ReturnClickedIndex()].data;
	
	render_says(click); // render the click
	
	if (click == simon_says[player.curr_step]){ // check if the click is in list
		if (player.max_step == MAX_SIMON_SAYS - 1){
			for (int i = 0; i < 50; i++){
				gfx_palette[0] = gfx_RGBTo1555(randInt(0, 255), randInt(0, 255), randInt(0, 255));
				gfx_Blit(1);
			}
			oxy_mouse.clicked_index = -3;
			return;
		}
			
		if (player.curr_step == player.max_step) { // 
			player.score++;
			render_score();
			
			if (player.max_step < MAX_SIMON_SAYS) {
				player.max_step++;
			}
			
			gfx_palette[0] = gfx_RGBTo1555(0, 255, 0); // Green
			gfx_Blit(1);
			delay(200);
			gfx_palette[0] = gfx_RGBTo1555(0, 0, 0);
			gfx_Blit(1);
			delay(100);
			
			player.curr_step = 0;
			render_pattern();
		}else{
			player.curr_step++;
		}
	}else{
		// lose (game over).
		gfx_palette[0] = gfx_RGBTo1555(255, 0, 0); // Red
		gfx_Blit(1);
		delay(200);
		gfx_palette[0] = gfx_RGBTo1555(0, 0, 0);
		gfx_Blit(1);
		
		player.wrong++;
		
		if (player.wrong == 3) {
			for (int i = 0; i < 20; i++){
				gfx_palette[0] = gfx_RGBTo1555(255, 0, 0); // Red
				gfx_Blit(1);
				gfx_palette[0] = gfx_RGBTo1555(0, 0, 0);
				gfx_Blit(1);
			}
			
			player.score = player.max_step = player.wrong = 0;
			render_score();
		}
		
		player.curr_step = 0; // start again
		render_pattern();
	}
}



