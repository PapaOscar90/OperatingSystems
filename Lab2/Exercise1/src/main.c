/// This application is a basic shell interpreter as defined in the provided
/// specification for the Operating Systems course.
///
/// https://github.com/hmgle/yascm was consulted as a resource on combining
/// `bison` and `flex` in an interactive c-application.
///
/// Its structure loosely follows the design in the MINIX book.
///    There is a loop which:
///        Displays a prompt.
///        Reads a line provided by a user.
///        Evaluates this line and returns the status of the shell as a result
///        of that evaluation.
///
///   The evaluation of the line first parses the line and if there were no
///   errors, then proceeds to execute the commands that were extracted from the
///   line.
///
///   The execution draws a distinction between a shell_builtin command and an
///   external command. The implementation of both is fairly simple and is
///   forwarded to sub-functions as needed.

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
