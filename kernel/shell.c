#include "shell.h"
#include "keyboard.h"
#include "terminal.h"
#include <stdio.h>
#include <string.h>

void execute(char *input) {
    if (strcmp(input, "help") == 0) {
        printf("Available commands: help, clear, exit\n");
    } else if (strcmp(input, "clear") == 0) {
        terminal_clear();
    } else {
        printf("Unknown command: %s\n", input);
    }
}

void shell_loop()
{
    char input[256];
    size_t index = 0;
    char c;

    while (1)
    {
        printf("> ");
        index = 0;

        while (1)
        {
            c = keyboard_get_char(); // Waits for input

            if (c == '\n' || c == '\r')  // Enter key
            {
                input[index] = '\0';
                printf("\n");
                break;
            }
            else if (c == '\b' || c == 127) // Backspace
            {
                if (index > 0)
                {
                    index--;
                    printf("\b \b"); // Erase last char on screen
                }
            }
            else
            {
                if (index < sizeof(input) - 1) {
                    input[index++] = c;
                    putchar(c);
                }
            }
        }

        if (strcmp(input, "exit") == 0)
            break;

        execute(input);
    }
}
