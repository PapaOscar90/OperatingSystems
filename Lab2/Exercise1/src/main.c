#include "shell.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// The shell follows the basic structure as provided in the Minix book:
int main() {

  while (true) {  // repeat forever
    typePrompt(); // display prompt on the screen
    readCommand();
  }

  return EXIT_SUCCESS;
}
