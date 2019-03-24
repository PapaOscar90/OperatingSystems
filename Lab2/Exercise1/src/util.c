#include "util.h"
#include "checked.h"
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

/// Read a line delimited by a newline from `stdin` and store it in `line`.
/// The delimiting newline is not included.
void read_line(char **line, size_t *line_len) {
  getline(line, line_len, stdin);
  *line_len = strlen(*line) - 1;
  (*line)[*line_len] = '\0';
}

// Go through the user's path from getenv to check if command exists
char *find_command_path(char *command) {
  // Get the user's path
  //
  // NOTE If a copy is not used, due to our code subsequent calls to
  // getenv("PATH") returns only the first directory on the path.
  // FIXME figure out why.
  char *temp = getenv("PATH");
  size_t len = strlen(temp) + 1;
  char *path = checked_malloc(len);
  strcpy(path, temp);
  path[len] = '\0';

  char *absolute_path;

  struct stat buffer;

  char *token = strtok(path, ":");
  // Go through all the paths appending the command.
  //   If the path exists, then break returning that path.
  while (token != NULL) {
    absolute_path = checked_calloc(strlen(token) + strlen(command) + 2,
                                   sizeof(*absolute_path));
    // Append the command to the directory
    sprintf(absolute_path, "%s/%s", token, command);

    // If that path exists and is a regular file.
    if (stat(absolute_path, &buffer) == 0 && S_ISREG(buffer.st_mode)) {
      free(path);
      return absolute_path;
    }

    // Get the next directory from the path
    token = strtok(NULL, ":");
    // Cleanup
    free(absolute_path);
  }

  free(path);
  // The command is not in the user's path
  return NULL;
}
