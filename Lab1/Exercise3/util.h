#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int readLine(char *str, int n, FILE *stream) {
  char *ptr = fgets(str, n, stream);
  if (ptr == NULL) {
    // TODO print the name of the stream in the error.
    fprintf(stderr, "Error when reading line.\n");
    exit(EXIT_FAILURE);
  }
  int strLength = strlen(str);
  strLength--;
  str[strLength] = '\0';
  return strLength;
}

#endif /* UTIL_H */
