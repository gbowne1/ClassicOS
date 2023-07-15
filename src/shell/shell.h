#ifndef SHELL_H
#define SHELL_H

// Function to read user input
char *read_input();

// Function to parse user input
char **parse_input(char *input);

// Function to execute a command
int execute_command(char **args);

#endif