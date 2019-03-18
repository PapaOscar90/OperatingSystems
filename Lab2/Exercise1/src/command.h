#ifndef COMMAND_H
#define COMMAND_H

#include "redirection.h"
#include <stdbool.h>

typedef enum { BUILTIN, EXTERNAL } CommandType;

typedef enum { EXIT } BuiltInType;

typedef struct { // COMMAND
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
