#ifndef UTIL_H
#define UTIL_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/// Read a line delimited by a newline from `stdin` and store it in `line`.
/// The delimiting newline is not included.
void read_line(char **line, size_t *line_len);

/// Return whether a character is quote or not (' or ").
bool is_quote(char const c);
/// Find the absolute path of a command if it exists.
/// The result is NULL if the command is not on the user's path.
char *find_command_path(char *command);

#endif /* UTIL_H */
