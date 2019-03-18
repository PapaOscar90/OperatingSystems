#ifndef SHELL_H
#define SHELL_H

#include "command.h"
#include "parse.h"
#include "redirection.h"
#include "util.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/// Evaluate the provided string, thereby executing the encoded commands.
void eval(char const *eval_string);

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

#endif /* SHELL_H */
