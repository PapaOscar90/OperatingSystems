#include "checked.h"
#include <stdio.h>
#include <unistd.h>

/// Malloc which panics on out-of-memory.
void *checked_malloc(size_t size) {
  void *p = malloc(size);
  if (p == NULL) {
    perror("malloc");
    exit(EXIT_FAILURE);
  }

  return p;
}

/// Calloc which panics on out-of-memory.
void *checked_calloc(size_t nmemb, size_t size) {
  void *p = calloc(nmemb, size);
  if (p == NULL) {
    perror("calloc");
    exit(EXIT_FAILURE);
  }

  return p;
}

/// Fork which exits on error.
pid_t checked_fork(void) {
  pid_t id = fork();
  if (id < 0) {
    perror("fork");
    exit(EXIT_FAILURE);
  }

  return id;
}
