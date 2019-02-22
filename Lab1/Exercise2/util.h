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

extern inline int isNonBreakingSpace(char c) { return c == ' ' || c == '\t'; }

// Read at most n characters from a line from a provided file stream.
char *readLine(int n, FILE *stream) {
  char *str = safeCalloc(n, sizeof(*str));
  char *ptr = fgets(str, n, stream);
  if (ptr == NULL) {
    fprintf(stderr, "Error when reading line.\n");
    exit(EXIT_FAILURE);
  }

  size_t strLen = strlen(str);

  char *line;

  // If there is a trailing newline
  if (str[strLen - 1] == '\n') {
    // Replace it
    str[strLen - 1] = '\0';
  }

  // Determine new string length
  strLen = strlen(str);

  // Truncate the memory of the temporary string and store the result in line.
  line = safeCalloc(strLen + 1, sizeof(*line));
  strncpy(line, str, strLen);

  // Cleanup
  free(str);

  return line;
}

#endif /* UTIL_H */
