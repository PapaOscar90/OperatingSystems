#define DEBUG 1

#include "macros.h"

#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern FILE *yyin;

void eval(char const *eval_string) {
  size_t num_commands = 0;
  Command *commands = parse(eval_string, &num_commands);

  // For each command in commands
  for (size_t i = 0; i < num_commands; ++i) {
    exec(commands[i]);
  }

  free(commands);
}

Command *parse(char const *parse_string, size_t *num_parsed) {
  DBG("Parsing string: %s", parse_string);
  *num_parsed = 0;
  Command *commands = checked_calloc(0, sizeof(*commands));
  return commands;
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

int yyparse(Element **env);

static bool END = false;

void eof_handle(void) { END = true; }

static Element *create_element(ElementType type) {
  Element *element = checked_calloc(1, sizeof(*element));
  element->type = type;
  return element;
}

Element *make_command(char const *command, char const *arguments,
                      Redirection redirection, bool in_background) {
  Element *element = create_element(COMMAND);
  element->command.arguments = unquote_string(arguments);
  element->command.redirection = redirection;
  element->command.in_background = in_background;
  // SHELL_BUILTINS
  if (strcmp(command, "exit") == 0) {
    element->command.type = BUILTIN;
    element->command.builtin = EXIT;
  } else {
    element->command.type = EXTERNAL;
    element->command.external = unquote_string(command);
  }
  return element;
}
