//! This application is a basic shell interpreter.
//! This application is a basic shell interpreter as defined in the provided
//! specification for the Operating Systems course.
//!
//! https://github.com/hmgle/yascm was consulted as a resource on combining
//! `bison` and `flex` in an interactive c-application. However, we were unable
//! to get bison and flex hooked in, in a timely fashion. As a result we tried
//! to write a parser combinator for the grammar by hand.
//!
//! Its structure loosely follows the design in the MINIX book.
//!    There is a loop which:
//!        Displays a prompt.
//!        Reads a line provided by a user.

#include "checked.h"
#include "exec.h"
#include "shell.h"
#include "util.h"
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
    Exec exec = eval(line);
    if (exec.type == EXEC_EXIT) {
      break;
    }
  }

  // Cleanup resources.
  free(line);

  return EXIT_SUCCESS;
}
