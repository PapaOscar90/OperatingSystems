#ifndef PARSE_H
#define PARSE_H

#include "input_line.h"
#include <stdlib.h>

typedef enum {
  PARSE_ERR,
  PARSE_INPUT_LINE,
  PARSE_COMMAND,
  PARSE_IO_REDIRECTION,
  PARSE_COMMAND_LIST,
  PARSE_COMMAND_PART,
  PARSE_FILE_NAME,
  PARSE_COMMAND_NAME,
  PARSE_OPTIONS,
} ParseType;

typedef struct {
  ParseType type;
  union {
    struct { // PARSE_ERR
    };
    struct { // PARSE_INPUT_LINE
      InputLine input_line;
    };
    struct { // PARSE_COMMAND
      Command command;
    };
    struct { // PARSE_IO_REDIRECTION
      IoRedirection io_redirection;
    };
    struct { // PARSE_COMMAND_LIST
      CommandList command_list;
    };
    struct { // PARSE_COMMAND_PART
      CommandPart command_part;
    };
    struct { // PARSE_FILE_NAME
      char *file_name;
    };
    struct { // PARSE_COMMAND_NAME
      char *command_name;
    };
    struct { // PARSE_OPTIONS
      char *options;
    };
  };
} Parse;

char *strip_quotes(char const *string);

Parse parse(char const *input);

void free_parse(Parse parse);

/* Parser Combinator Functions ***********************************************/
Parse parse_input_line(char const *input, size_t start, size_t *new_position);

Parse parse_command(char const *input, size_t start, size_t *new_position);

Parse parse_io_redirection(char const *input, size_t start,
                           size_t *new_position);

Parse parse_command_list(char const *input, size_t start, size_t *new_position);

Parse parse_command_part(char const *input, size_t start, size_t *new_position);

Parse parse_file_name(char const *input, size_t start, size_t *new_position);

Parse parse_command_name(char const *input, size_t start, size_t *new_position);

Parse parse_options(char const *input, size_t start, size_t *new_position);

void skip_whitespace(char const *input, size_t start, size_t *new_position);
#endif /* PARSE_H */
