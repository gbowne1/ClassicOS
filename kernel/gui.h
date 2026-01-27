#ifndef GUI_H
#define GUI_H

#include <stdint.h>
#include <stddef.h>

#define GUI_WINDOW_WIDTH   80
#define GUI_WINDOW_HEIGHT  25
#define GUI_BUTTON_WIDTH   10
#define GUI_BUTTON_HEIGHT  3

// Window structure
typedef struct {
    uint32_t x, y;
    uint32_t width, height;
    uint32_t color;  // Background color
    const char* title;
} gui_window_t;

// Button structure
typedef struct {
    uint32_t x, y;
    uint32_t width, height;
    uint32_t color;  // Background color
    const char* label;
} gui_button_t;

// Function prototypes for GUI elements
void gui_init(void);
void gui_draw_window(gui_window_t* window);
void gui_draw_button(gui_button_t* button);
void gui_clear(uint32_t color);

#endif // GUI_H
