#ifndef COMMAND_H
#define COMMAND_H

#include "redirection.h"
#include <stdbool.h>

/// Possible command types.
typedef enum { BUILTIN, EXTERNAL } CommandType;

/// Possible shell built-ins.
typedef enum { EXIT } BuiltInType;

/// A command that may be executed
typedef struct {
  CommandType type;
  union {
    struct { // BUILTIN
      BuiltInType builtin;
    };
    struct { // EXTERNAL
      char *external;
    };
  };
  char *arguments;
  Redirection redirection;
  bool in_background;
} Command;

/// Create a command.
Command create_command(char *command_name, char *arguments,
                       Redirection redirection, bool in_background);

/// Free the managed memory components of a command.
void free_command(Command command);

#endif /* COMMAND_H */
