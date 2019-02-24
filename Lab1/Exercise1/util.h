#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// calloc which panics on OOM.
void *safeCalloc(size_t __nmemb, size_t __size) {
  void *p = calloc(__nmemb, __size);
  if (p == NULL) {
    fprintf(stderr, "Calloc failed. Out of memory?\n");
    exit(EXIT_FAILURE);
  }

  return p;
}

// realloc which panics on OOM.
void *safeRealloc(void *__ptr, size_t __size) {
  void *p = realloc(__ptr, __size);
  if (p == NULL) {
    fprintf(stderr, "Realloc failed. Out of memory?\n");
    exit(EXIT_FAILURE);
  }

  return p;
}

extern inline int isNonBreakingSpace(char c) { return c == ' ' || c == '\t'; }

// Read line from a file stream
char *readLine(FILE *stream) {
  size_t size = 0;
  size_t len = 0;
  size_t last = 0;
  char *buffer = NULL;

  do {
    size += BUFSIZ;
    buffer = safeRealloc(buffer, size);
    fgets(buffer + last, size, stream);
    len = strlen(buffer);
    last = len - 1;
  } while (!feof(stream) && buffer[last] != '\n');

  if (buffer[last] == '\n') {
    buffer[last] = '\0';
  }
  return buffer;
}

#endif /* UTIL_H */
