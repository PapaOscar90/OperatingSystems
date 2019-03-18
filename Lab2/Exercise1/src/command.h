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

#endif /* COMMAND_H */
