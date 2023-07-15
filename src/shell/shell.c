#include "shell.h"
#include <stdio.h>
#include <string.h>

void shell_loop()
{
    char input[256];

    while (1)
    {
        printf("> ");
        fgets(input, sizeof(input), stdin);

        // Remove newline character from input
        input[strcspn(input, "\n")] = '\0';

        // Parse input and execute commands
        // ...

        // Exit the shell if the user enters "exit"
        if (strcmp(input, "exit") == 0)
        {
            break;
        }
    }
}

int main()
{
    shell_loop();
    return 0;
}