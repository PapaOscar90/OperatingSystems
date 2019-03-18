#ifndef SHELL_H
#define SHELL_H

#include "command.h"
#include "redirection.h"
#include "util.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/// Evaluate the provided string, thereby executing the encoded commands.
void eval(char const *eval_string);

/// Parse the provided string, thereby extracting the encoded commands. The
/// extracted commands are stored along the returned pointer. The amount of
/// commands is updated in the reference to `num_commands`.
Command *parse(char const *parse_string, size_t *num_commands);

/// Execute a command. A `command` may be either a `BUILTIN` or an `EXTERNAL`
/// command. The logic is necessarily different depending on what should occur.
void exec(Command command);

/// Execute a builtin command.
void exec_builtin(Command command);

/// Execute an external command.
void exec_external(Command command);

/// Display a prompt to the user.
void type_prompt(void);

/* SHELL PARSING *************************************************************/
typedef enum {
  COMMAND_NAME,
  OPTIONS,
  COMMAND_PART,
  COMMAND_LIST,
  REDIRECT_IN,
  REDIRECT_OUT,
  IO_REDIRECTION,
  EMPTY,
  FILE_NAME,
  COMMAND,
  BACKGROUND,
  INPUT_LINE,
  OTHER,
} ElementType;

typedef struct {
  ElementType type;
  union { // Allows for other elements to be specified in the future
    Command command;
  };
} Element;

// Create a command as one of the possible parsed elements.
Element *make_command(char const *command, char const *arguments,
                      Redirection redirection, bool in_background);

// Deal with the EOF in the parsing.
void eof_handle(void);

#endif /* SHELL_H */
