#ifndef COMMAND_LIST_H
#define COMMAND_LIST_H

#include "command_part.h"
#include "exec.h"
#include "io_redirection.h"
#include <stdlib.h>

typedef struct {
  size_t amount;
  CommandPart *command_parts;
} CommandList;

void free_command_list(CommandList command_list);
CommandList new_command_list(size_t amount);
Exec exec_command_list(CommandList command_list, IoRedirection io_redirection);

#endif /* COMMAND_LIST_H */
