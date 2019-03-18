#include "command.h"
#include "redirection.h"
#include <stdlib.h>
#include <string.h>

void free_command(Command command) {
  free_redirection(command.redirection);
  free(command.arguments);
  switch (command.type) {
  case BUILTIN:
    // Nothing to free
    break;
  case EXTERNAL:
    free(command.external);
    break;
  }
}

Command create_command(char *command_name, char *arguments,
                       Redirection redirection, bool in_background) {
  Command command;
  command.arguments = arguments;
  command.redirection = redirection;
  command.in_background = in_background;
  // SHELL_BUILTINS
  if (strcmp(command_name, "exit") == 0) {
    command.type = BUILTIN;
    command.builtin = EXIT;
  } else {
    command.type = EXTERNAL;
    command.external = command_name;
  }
  return command;
}
