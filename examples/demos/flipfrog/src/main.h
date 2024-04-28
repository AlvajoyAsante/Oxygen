#ifndef MAIN_H
#define MAIN_H

// There was no reason for a main.h it's just here to here developers understand external functions

#define FROG_VER "DEMO"
#define DEV_INFO "CREATED BY ALVAJOY 'ALVAJOY123' ASANTE"
#define DEV_INFO_L1 "THANKS TO:"
#define DEV_INFO_L2 "MateoC, Commandz, Michael0x18"
#define DEV_MODE 0
// ^^^ CHANGE TO 1 TO ENABLE DEV_MODE ex. "#define DEV_MODE 1"

#define TIMER_FREQ      32768 /* Frequency of timer in Hz */
#define ONE_SECOND      (TIMER_FREQ / 1)
#define HALF_SECOND     (TIMER_FREQ / 2)
#define QUARTER_SECOND  (TIMER_FREQ / 4)
#define CHANGE_TIME (TIMER_FREQ / 1) * 1


#include <tice.h>

struct frog_data_t{ // just basic data for the frog
	uint24_t x;
	uint8_t	y;
	
	int timer;
	int costume;
	
	uint8_t color[3];
	
	int size;
	int animation;
	
	int ret_animation;
	bool rotated;
};
extern struct frog_data_t frog_data;

struct menu_data_t{ // when a user clicks a menu on these variables are updated "look in more_menu_clicked(); or mouse_enter(); for more info"
	int change_menu;
	int more_menu;
};
extern struct menu_data_t menu_data;


// Functions (Note: The functions are all spaced out making it easy it read)
void frog_costume(void);
void frog_costume_draw(void);

void UI_setup(void);

void more_menu_clicked(void);
void mouse_enter(int index);

uint8_t change_frog_colors(uint8_t curr_color);

void active_hover_index(int index);
void remove_hover_index(int index);

void vars_init(void);
void DEBUG(void);
#endif