#ifndef GUI_H
#define GUI_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

// Forward declarations
struct Window;
enum EventType;

#define VGA_MODE 0x800x600
#define VGA_WIDTH 640
#define VGA_HEIGHT 480
#define VGA_BPP 32

// Event structure
typedef struct {
    enum EventType type;
    union {
        struct {
            int key_code;
        } key_press;
        struct {
            int x, y;
        } mouse_move;
        // Add more event types as needed
    };
} Event;

// Function prototypes
void init_gui(void);
void render_framebuffer(void* fb);
void handle_events(void);
struct Window* create_window(int x, int y, int width, int height, uint32_t color);
void destroy_window(struct Window* win);
void update_window(struct Window* win);
void draw_text(const char* text, int x, int y);
void draw_button(const char* label, int x, int y, uint32_t color);
void handle_event(Event* ev);

// Global variables
extern void* framebuffer;
extern int fb_pitch;
extern int fb_fd;

// Custom OS-specific functions
void init_video_mode(void);
void get_framebuffer_info(void);
void set_custom_video_mode(void);
// Helper functions
void print_error(const char* message);
void check_mmap_result(void* ptr, const char* filename);
void check_open_result(int fd, const char* filename);
void check_ioctl_result(int result, const char* func_name);

// Custom OS-specific structures
typedef struct {
    int x, y;
    int width, height;
    uint32_t color;
    void* address; // Address of the frame buffer
    int pitch;     // Pitch of the frame buffer
} Window;

#endif // GUI_H
