#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>

// Display the shell prompt to the user.
void typePrompt(void);

// Read a line from a file stream
char *readLine(FILE *stream);

// Read the command from the terminal
void readCommand();

#endif /* SHELL_H */
