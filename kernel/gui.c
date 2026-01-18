#include "gui.h"
#include "vga.h"  // VGA functions for drawing and clearing screen
#include "framebuffer.h"  // For pixel manipulation if needed

// Initialize the GUI (could set up any global state or variables here)
void gui_init(void) {
    // Clear the screen with black or any color
    gui_clear(vga_entry_color(VGA_COLOR_BLACK, VGA_COLOR_WHITE));
}

// Draw a window (simple rectangle with a title)
void gui_draw_window(gui_window_t* window) {
    // Draw the window's border
    for (uint32_t y = 0; y < window->height; ++y) {
        for (uint32_t x = 0; x < window->width; ++x) {
            // Check if we are at the border
            if (x == 0 || y == 0 || x == window->width - 1 || y == window->height - 1) {
                vga_put_entry_at('#', vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK), window->x + x, window->y + y);
            } else {
                // Fill the inside of the window
                vga_put_entry_at(' ', vga_entry_color(VGA_COLOR_BLACK, VGA_COLOR_BLACK), window->x + x, window->y + y);
            }
        }
    }

    // Draw the title at the top
    if (window->title) {
        size_t i = 0;
        while (window->title[i] != '\0' && i < window->width - 2) {
            vga_put_entry_at(window->title[i], vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK), window->x + i + 1, window->y);
            i++;
        }
    }
}

// Draw a button (a simple rectangle with text in the middle)
void gui_draw_button(gui_button_t* button) {
    for (uint32_t y = 0; y < button->height; ++y) {
        for (uint32_t x = 0; x < button->width; ++x) {
            // Check if we are at the border
            if (x == 0 || y == 0 || x == button->width - 1 || y == button->height - 1) {
                vga_put_entry_at('#', vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK), button->x + x, button->y + y);
            } else {
                // Fill the inside of the button
                vga_put_entry_at(' ', vga_entry_color(VGA_COLOR_BLACK, VGA_COLOR_BLACK), button->x + x, button->y + y);
            }
        }
    }

    // Draw the label in the center of the button
    size_t label_len = 0;
    while (button->label[label_len] != '\0') {
        label_len++;
    }

    size_t start_x = button->x + (button->width - label_len) / 2;
    size_t start_y = button->y + (button->height - 1) / 2;
    for (size_t i = 0; i < label_len; ++i) {
        vga_put_entry_at(button->label[i], vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK), start_x + i, start_y);
    }
}

// Clear the screen with a color
void gui_clear(uint32_t color) {
    vga_clear(color);  // Just clear the VGA screen with a solid color
}
