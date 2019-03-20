//! A module wrapping common functions to crash on error instead of requiring
//! checks at the call site.

#ifndef CHECKED_H
#define CHECKED_H

#include <stdlib.h>

/// Calloc which panics on out-of-memory.
void *checked_calloc(size_t nmemb, size_t size);

/// Malloc which panics on out-of-memory.
void *checked_malloc(size_t size);

/// Fork which exits on error.
pid_t checked_fork(void);

#endif /* CHECKED_H */
