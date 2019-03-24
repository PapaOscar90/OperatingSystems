#ifndef COMMAND_H
#define COMMAND_H

#include "command_list.h"
#include "exec.h"
#include "io_redirection.h"
#include <stdbool.h>

typedef struct {
  CommandList command_list;
  IoRedirection io_redirection;
} Command;

Exec exec_command(Command command, bool in_background);

void free_command(Command command);

#endif /* COMMAND_H */
