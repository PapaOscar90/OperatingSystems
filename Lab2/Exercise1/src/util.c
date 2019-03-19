#define DEBUG 1

#include "macros.h"

#include "util.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

void *checked_malloc(size_t size) {
  void *p = malloc(size);
  if (p == NULL) {
    fprintf(stderr, "Malloc failed, out of memory?\n");
    exit(EXIT_FAILURE);
  }

  return p;
}

void *checked_calloc(size_t nmemb, size_t size) {
  void *p = calloc(nmemb, size);
  if (p == NULL) {
    fprintf(stderr, "Calloc failed, out of memory?\n");
    exit(EXIT_FAILURE);
  }

  return p;
}

void read_line(char **line, size_t *line_len) {
  getline(line, line_len, stdin);
  *line_len = strlen(*line) - 1;
  (*line)[*line_len] = '\0';
}

char *unquote_string(char const *string) {
  char *intermediary = checked_malloc(strlen(string) + 1);
  size_t result_len = 0;
  bool in_quote = false;
  char quote = '\0';

  for (size_t i = 0; i < strlen(string); ++i) {
    if (in_quote && string[i] == quote) {
      in_quote = false;
      quote = '\0';
      continue;
    } else if (!in_quote && (string[i] == '\'' || string[i] == '"')) {
      in_quote = true;
      quote = string[i];
      continue;
    }

    intermediary[result_len++] = string[i];
  }

  char *result = checked_malloc(result_len);
  strncpy(result, intermediary, result_len);
  result[result_len] = '\0';

  free(intermediary);

  if (in_quote) {
    free(result);
    return NULL;
  } else {
    return result;
  }
}

// fork which exits on error
pid_t checked_fork(void) {
  pid_t id = fork();
  if (id == -1) {
    perror("fork");
    exit(EXIT_FAILURE);
  }
  return id;
}

// Go through the user's path from getenv to check if command exists
char *find_command_path(char *command) {
  // Get the user's path
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
