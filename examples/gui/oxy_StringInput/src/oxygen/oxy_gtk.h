#ifndef OXY_OXY_H
#define OXY_OXY_H


// K.I.S.S. PRINCIPLE

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define OXY_NUM_TYPE		0
#define OXY_TEXT_TYPE		1
#define OXY_SPRITE_TYPE		2
#define OXY_BUTTON_TYPE		3
#define OXY_WINDOW_TYPE		4
#define OXY_DIVIDER_TYPE	5

/* Color of gtk objects */
struct OXY_color_t {
	uint8_t text_bg;
	uint8_t text_fg;
	
	uint8_t color_a;
	uint8_t color_b;
};

/* State of an object "derived from gtk" */
struct OXY_state_t {
	bool visible;
	bool clicked;
	bool clickable;
};

/* Array that contains Position and offsets */
struct OXY_position_t {
	int x;
    int y;
};

/* array that contains width and height data */
struct OXY_sizeinfo_t {
    int width;
    int height;
};

/* The widget is the base of the tree for displayable objects. */
struct OXY_widget_t {
	uint8_t type;
	
	struct OXY_color_t color;
	struct OXY_position_t position;
	struct OXY_sizeinfo_t size;
	struct OXY_state_t state;
	
	void (*update)(struct OXY_widget_t *);
	void (*render)(struct OXY_widget_t *);
    
	struct OXY_widget_t **child;
};

/* Window Array */
struct OXY_window_t {
	char *title;
	char *wm_class;
	
	struct OXY_widget_t widget;	
	struct OXY_color_t colors;
};

/* Window management system */
struct OXY_wm_t {
	uint8_t amount;
};
extern struct OXY_wm_t OXY_wm;


/**
 * Gets string input from user.
 * @param title Input text (text displayed before input).
 * @param x X coordinate
 * @param y Y coordinate
 * @param width Max input characters
 * @param height Max input characters
 * @returns OXY_window_t* Return users input
 */
struct OXY_window_t* oxy_CreateWindow(char title[9], int x, uint8_t y, int width, int height);

void oxy_UpdateWindow(struct OXY_window_t *window);

void oxy_RenderWindow(struct OXY_window_t *window);

void oxy_CenteredWindow(char *title, int width, int height);

void oxy_SetWindowColor(struct OXY_window_t *window, uint8_t boarder_color, uint8_t center_color);

void oxy_SetWindowTextColor(struct OXY_window_t *window, uint8_t text_fg, uint8_t text_bg);

void oxy_Message(char *title, char *message);

void oxy_PrintWordWrap(char *text, uint24_t x, uint8_t y, int width, uint8_t max_lines, uint8_t init_line);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __OXY_OXY_H__ */