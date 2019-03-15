#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Calloc which panics on OOM.
void *safe_calloc(size_t nmemb, size_t size) {
  void *p = calloc(nmemb, size);
  if (p == NULL) {
    fprintf(stderr, "Calloc failed. Out of memory?\n");
    exit(EXIT_FAILURE);
  }
  return p;
}

// Read a line delimited by a newline from stdin. The newline is not included.
void read_line(char **line, size_t *line_len) {
  // Read a line from stdin
  getline(line, line_len, stdin);
  (*line_len) = strlen(*line) - 1;
  (*line)[(*line_len)] = '\0';
}

#endif /* UTIL_H */
