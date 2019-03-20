#include "command_part.h"
#include <stdlib.h>

void free_command_part(CommandPart command_part) {
  free(command_part.command_name);
  free(command_part.options);
}
