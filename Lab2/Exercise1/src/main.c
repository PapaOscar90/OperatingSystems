#include "shell.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
  // Disable buffering of `stdout`.
  setbuf(stdout, NULL);

  // Initialize the variables to hold a line read from `stdin`.
  size_t line_len = 1;
  char *line = checked_calloc(line_len, sizeof(*line));

  while (true) {
    // Display a prompt to the user.
    type_prompt();
    // Read the line provided by the user.
    read_line(&line, &line_len);
    // Evaluate the line.
    eval(line);
  }

  // Cleanup resources.
  free(line);

  return EXIT_SUCCESS;
}
