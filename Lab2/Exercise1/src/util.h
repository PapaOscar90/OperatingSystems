#ifndef UTIL_H
#define UTIL_H

#include <stdlib.h>

/// Calloc which panics on out-of-memory.
void *checked_calloc(size_t nmemb, size_t size);

/// Malloc which panics on out-of-memory.
void *checked_malloc(size_t size);

/// Fork which exits on error.
pid_t checked_fork(void);

/// Strip pairs of quotes from a string. A quote of a different type when within
/// two other quotes is maintained. The returned string is managed memory, and
/// thus must be freed by the caller.
///
/// ASSUMPTIONS:
///    All quotes come in pairs. Passing in a string with mismatched quotes is
///    considered UB. This could behavior could be rectified later to return
///    NULL, specifically raise an error, or crash.
char *unquote_string(char const *string);

/// Read a line delimited by a newline from `stdin` and store it in `line`. The
/// terminating newline is not included.
void read_line(char **line, size_t *line_len);

/// Find the absolute path of a command if it exists.
/// The result is NULL if the command is not on the user's path.
char *find_command_path(char *command);

#endif /* UTIL_H */
