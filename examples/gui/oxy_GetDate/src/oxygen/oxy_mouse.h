#ifndef OXY_MOUSE_H
#define OXY_MOUSE_H

// K.I.S.S. PRINCIPLE

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

// Variables
/**
 * @brief Holds information about mouse and hoverspots.
 */ 
struct oxy_mouse_t {
	int x;
	int y;
	
	uint8_t speed;
	
	int scroll_X;
	int scroll_Y;
	
	uint8_t hover_amount; 
	int clicked_index;
};
extern struct oxy_mouse_t oxy_mouse;

/**
 * @brief Holds information about hoverspot (dynamic pointer).
 */ 
struct oxy_detect_t{
	uint16_t x;
	uint8_t y; 
	
	uint16_t w; 
	uint8_t h;
	
	uint8_t data; // any number data you want to use.
	void (*right_click)(void *); // function for clicking.
	void *right_arg; // arguments for "click"
	
	void (*left_click)(void *); // function for clicking.
	void *left_arg; // arguments for "click"
	
	char *description;
};
extern struct oxy_detect_t *oxy_detect;

// Functions
/**
 * @brief Init all variable needed for Mouse file.
 */ 
void oxy_InitMouse(void);

void oxy_ResetMouse(void);

/**
 * Creates a Hover spot. 
 * @param x X position of hoverspot. 
 * @param y Y position of hoverspot.
 * @param w Width of hoverspot.
 * @param h Height of hoverspot.
 * @returns Returns the index of the hover spot.
 */ 
uint8_t oxy_AddHover(uint16_t x, uint8_t y, uint16_t w, uint8_t h);

/**
 * Removes Hover at set index
 * @param index Set this to hover spot index. (returned when hoverspot was added) 
 * @returns true if index was deleted and false for error. 
 */ 
bool oxy_RemoveHover(uint8_t index);

/**
 * @brief Removes all hover spots   
 */ 
void oxy_RemoveAllHover(void);

void oxy_RemoveAllBelow(uint8_t index);

void oxy_SetRightClick(void *function, void *arg, uint8_t index);
void oxy_SetLeftClick(void *function, void *arg, uint8_t index);
void oxy_SetMouseSpeed(uint8_t speed);

/**
 * Creates a description at index, if mouse detects hover spot with description. 
 * The description is placed over the mouse pointer.
 * @param text Hover Spot Description .
 * @param index Set this to hover spot index. (returned when hoverspot was added)
 */ 
bool oxy_SetHoverDescription(char text[], uint8_t index);

int oxy_ReturnClickedIndex(void);

/**
 * @brief Detects if mouse is over a hover spot
 */ 
int oxy_DetectHover(void);

/**
 * @brief Renders Mouse to Blit(1).  
 */ 
void oxy_RenderMouse(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif