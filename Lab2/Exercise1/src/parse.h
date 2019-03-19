#ifndef PARSE_H
#define PARSE_H

#include "command.h"
#include <stdlib.h>

/// Possible results when parsing.
typedef enum {
  COMMAND_NAME,
  OPTIONS,
  COMMAND_PART,
  IO_REDIRECTION,
  COMMAND,
  INPUT_LINE,
  EOI,
  ERR,
} ParseResultType;

/// Intermediary command part
typedef struct {
  char *command_name;
  char *options;
} CommandPart;

/// Result type containing possible parse results and their corresponding
/// values;
typedef struct {
  ParseResultType type;
  union {
    struct { // COMMAND_NAME
      char *command_name;
    };
    struct { // OPTIONS
      char *options;
    };
    struct { // COMMAND_PART
      CommandPart command_part;
    };
    struct { // IO_REDIRECTION
      Redirection redirection;
    };
    struct { // COMMAND
      Command command;
    };
    struct { // INPUT_LINE
      Command *commands;
    };
  };
} ParseResult;

/// Parse the provided string, thereby extracting the encoded commands. The
/// extracted commands are stored along the returned pointer. The amount of
/// commands is updated in the reference to `num_commands`.
Command *parse(char const *parse_string, size_t *num_commands);

/// Set `new_position` to the first non-whitespace character from a starting
/// index.
/// If the start position is outside the bounds of the string
/// `new_position` is left unchanged.
/// If the string is only composed of whitespace characters from the start
/// position to the end of the string, `new_position` is set to the end of the
/// string.
void skip_whitespace(char const *parse_string, size_t start,
                     size_t *new_position);

/// Parse a command name from a string.
/// If successful the new position is the next character from the end of the
/// string.
ParseResult parse_command_name(char const *parse_string, size_t start,
                               size_t *new_position);

/// Parse options from a string.
/// If successful the new position is the next character from the end of the
/// string.
/// If there are no options the parsed value for the options is NULL.
ParseResult parse_options(char const *parse_string, size_t start,
                          size_t *new_position);

#endif /* PARSE_H */
