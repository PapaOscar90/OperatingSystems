#ifndef PARSE_H
#define PARSE_H

#include "command.h"
#include <stdlib.h>

/// Parse the provided string, thereby extracting the encoded commands. The
/// extracted commands are stored along the returned pointer. The amount of
/// commands is updated in the reference to `num_commands`.
Command *parse(char const *parse_string, size_t *num_commands);

#endif /* PARSE_H */
