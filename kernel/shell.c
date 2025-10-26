#include "shell.h"
#include "keyboard.h"
#include "terminal.h"
#include "print.h"
#include "string_utils.h"

void execute(char *input) {
    if (my_strcmp(input, "help") == 0) {
        my_printf("Available commands: help, clear, exit\n");
    } else if (my_strcmp(input, "clear") == 0) {
        terminal_clear();
    } else {
        my_printf("Unknown command: %s\n", input);
    }
}

void shell_loop()
{
    char input[256];
    size_t index = 0;
    char c;

    while (1)
    {
        my_printf("> ");
        index = 0;

        while (1)
        {
            c = keyboard_get_char(); // Waits for input

            if (c == '\n' || c == '\r')  // Enter key
            {
                input[index] = '\0';
                my_printf("\n");
                break;
            }
            else if (c == '\b' || c == 127) // Backspace
            {
                if (index > 0)
                {
                    index--;
                    my_printf("\b \b"); // Erase last char on screen
                }
            }
            else
            {
                if (index < sizeof(input) - 1) {
                    input[index++] = c;
                    terminal_putchar(c);
                }
            }
        }

        if (my_strcmp(input, "exit") == 0)
            break;

        execute(input);
    }
}
