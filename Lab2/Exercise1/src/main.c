#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

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

// The shell follows the basic structure as provided in the Minix book:
int main() {

  while (true) {  // repeat forever
    typePrompt(); // display prompt on the screen
    readCommand();
  }

  return EXIT_SUCCESS;
}
