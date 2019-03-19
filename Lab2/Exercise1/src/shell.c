#define DEBUG 1

#include "macros.h"

#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef LINE_MAX
#define LINE_MAX 10000000
#endif

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
  DBG("Command: %s", command.external);
  DBG("Arguments: %s", command.arguments);
  DBG("In background?: %s", command.in_background ? "true" : "false");
  DBG("Redirection: %d", command.redirection.type);

  // Find the command's absolute path
  char *commandPath = findCommandPath(command.external);

  int child;
  child = fork();
  if (child < 0) {
    fprintf(stderr, "Fork failed: aborted\n");
    return EXIT_FAILURE;
  }

  char *commandPath = findCommandPath(command);
  if (commandPath == NULL) {
    // If the command could not be found on the user's path
    printf("Command %s not found!\n", command);
    free(commandPath);
    return EXIT_SUCCESS;
  }

  if (child == 0) {
    char *newargv[3];
    newargv[0] = command.external;
    newargv[1] = command.arguments;
    newargv[2] = NULL;

    execve(commandPath, command.arguments, NULL);
  }

  if (!command.in_background) {
    int status;
    waitpid(-1, &status, 0);
  }

  free(commandPath);
  free(command.arguments);
  free(command.external);
}

void type_prompt() {
  // NOTE Currently no prompt displayed.
}
