#define DEBUG 1

#include "macros.h"

#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void eval(char const *eval_string) {
  size_t num_commands = 0;
  Command *commands = parse(eval_string, &num_commands);

  // For each command in commands
  for (size_t i = 0; i < num_commands; ++i) {
    exec(commands[i]);
  }

  free(commands);
}

// TODO should this return the exit_code along side the inevitable shell_status?
void exec(Command command) {
  switch (command.type) {
  case BUILTIN:
    return exec_builtin(command);
  case EXTERNAL:
    return exec_external(command);
  }
}

void exec_builtin(Command command) {
  DBG("Executing builtin command...");
  DBG("Command: %u", command.builtin);
  DBG("Arguments: %s", command.arguments);
  DBG("In background?: %s", command.in_background ? "true" : "false");
  DBG("Redirection: %d", command.redirection.type);

  // If in background spawn child to run command.
  // If with redirection spawn fds and pipes to pass in and pass out results.
  // Note as the command is a shell builtin we need not consult the path for its
  // existence. Execute command with arguments
}

void exec_external(Command command) {
  DBG("Executing external command...");
  DBG("Command: %s", command.external);
  DBG("Arguments: %s", command.arguments);
  DBG("In background?: %s", command.in_background ? "true" : "false");
  DBG("Redirection: %d", command.redirection.type);

  // If in background spawn child to run command.
  // If with redirection spawn fds and pipes to pass in and pass out results.
  // Check if command is on path. If not return error.
  // If is on path, execute command with arguments.
}

void type_prompt() {
  // NOTE Currently no prompt displayed.
}
