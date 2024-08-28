#include "gui.h"

int main() {
    init_gui();

    // Create a window
    Window* win = create_window(50, 50, 100, 100, 0xFF0000);

    // Draw something
    draw_text("Hello, World!", 60, 60, 0xFFFFFF);

    // Handle events
    handle_events();

    // Keep the program running
    while (1) {
        usleep(10000); // Sleep for 10ms
    }

    return 0;
}