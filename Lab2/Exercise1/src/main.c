#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// Display the shell prompt to the user.
// NOTE this could be extended to allow for user customization.
void typePrompt(void) { printf("$ "); }

// The shell follows the basic structure as provided in the Minix book:
int main() {

  while (true) {  // repeat forever
    typePrompt(); // display prompt on the screen
  }

  return EXIT_SUCCESS;
}
