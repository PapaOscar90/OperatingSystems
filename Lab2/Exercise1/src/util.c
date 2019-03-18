#include "util.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
  return result;
}
