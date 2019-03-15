#include "shell.h"
#include "util.h"

int main() {
  // Disable buffering of stdout
  setbuf(stdout, NULL);

  // Initialize the shell status
  ShellStatus shell_status = ShellStartup;

  // Initialize the variables to hold a line from stdin
  size_t line_len = 1;
  char *line = safe_calloc(line_len, sizeof(*line));

  while (shell_status != ShellExit) {
    // Display a prompt the user
    type_prompt();
    // Read the line provided by the user
    read_line(&line, &line_len);
    // Evaluate the line and report the shell status
    shell_status = eval(line);

    // If there is an error display it
    if (shell_status < 0) {
      print_shell_status(shell_status);
    }
  }

  // Cleanup resources
  free(line);

  return 0;
}
