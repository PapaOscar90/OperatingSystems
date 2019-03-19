#define DEBUG 1

#include "macros.h"

#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

void eval(char const *eval_string) {
  size_t num_commands = 0;
  char *parse_string = unquote_string(eval_string);
  Command *commands = parse(parse_string, &num_commands);

  // For each command in commands
  for (size_t i = 0; i < num_commands; ++i) {
    exec(commands[i]);
  }

  free(commands);
  free(parse_string);
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
  switch (command.builtin) {
  case EXIT:
    break;
  }

  // If in background spawn child to run command.
  // If with redirection spawn fds and pipes to pass in and pass out results.
  // Note as the command is a shell builtin we need not consult the path for its
  // existence. Execute command with arguments
}

void exec_external(Command command) {
  // If in background spawn child to run command.
  // If with redirection spawn fds and pipes to pass in and pass out results.
  // Check if command is on path. If not return error.
  // If is on path, execute command with arguments.
  DBG("Executing external command...");
  print_command(command);

  // Find the command's absolute path
  char *command_path = find_command_path(command.external);
  if (command_path == NULL) {
    DBG("command not on path");
    // TODO propogate error.
    return;
  }

  // Execute the command.
  pid_t child = checked_fork();
  if (child != 0) {
    // Parent

    // If the process is in the foreground, wait for the child;
    if (!command.in_background) {
      waitpid(child, NULL, 0);
    }
  } else {
    // Child
    char *newargv[3];
    newargv[0] = command.external;
    newargv[1] = command.arguments;
    newargv[2] = NULL;

    execve(command_path, newargv, NULL);
  }

  free(command_path);
}

void type_prompt() {
  // NOTE Currently no prompt displayed.
}
