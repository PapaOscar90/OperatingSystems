#include "command.h"
#include "redirection.h"
#include <stdio.h>
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

void print_command(Command command) {
  printf("COMMAND:\n");
  switch (command.type) {
  case BUILTIN:
    printf("\t command (builtin): '%d'\n", command.builtin);
    break;
  case EXTERNAL:
    printf("\t command (external): '%s'\n", command.external);
    break;
  }
  printf("\t arguments: '%s'\n", command.arguments);
  printf("\t redirection: '%d'\n", command.redirection.type);
  printf("\t in_background: '%s'\n", command.in_background ? "true" : "false");
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
