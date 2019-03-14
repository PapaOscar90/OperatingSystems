#include "shell.h"

// Display the shell prompt to the user.
// NOTE this could be extended to allow for user customization.
void typePrompt(void) { printf("$ "); }

// TODO Read a line from a file stream
char *readLine(FILE *stream) {
  char c;
  fscanf(stream, "%c\n", &c);

  fprintf(stderr, "Unimplemented\n");
  exit(EXIT_FAILURE);
}

// TODO Read the command from the terminal
void readCommand() { char *input = readLine(stdin); }
