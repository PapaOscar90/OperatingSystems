#define DEBUG 1

#include "macros.h"

#include "command.h"
#include "command_vec.h"
#include "parse.h"
#include "redirection.h"
#include "util.h"
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

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

typedef struct {
  char *command_name;
  char *options;
} CommandPart;

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

void free_parse_result(ParseResult result) {
  switch (result.type) {
  case ERR:
    break;
  case EOI:
    break;
  case COMMAND_PART:
    free(result.command_part.command_name);
    free(result.command_part.options);
  case COMMAND_NAME:
    free(result.command_name);
    break;
  case OPTIONS:
    free(result.options);
    break;
  case IO_REDIRECTION:
    free_redirection(result.redirection);
    break;
  case COMMAND:
    free_command(result.command);
    break;
  case INPUT_LINE:
    fprintf(stderr, "Unreachable: %s:%d%s\n", __FILE__, __LINE__, __func__);
    exit(EXIT_FAILURE);
  }
}

ParseResult parse_command_name(char const *parse_string, size_t start,
                               size_t *new_position) {
  bool in_quote = false;
  size_t result_len = 0;
  char quote = '\0';

  char *intermediary = checked_malloc(strlen(parse_string) + 1);

  for (; *new_position < strlen(parse_string); (*new_position)++) {
    if (in_quote && parse_string[*new_position] == quote) {
      in_quote = false;
      quote = '\0';
    } else if (!in_quote && (parse_string[*new_position] == '\'' ||
                             parse_string[*new_position] == '"')) {
      in_quote = true;
      quote = parse_string[*new_position];
    } else if (!in_quote && !isspace(parse_string[*new_position])) {
      break;
    }
  }

  fprintf(stderr, "Unimplemented\n");
  exit(EXIT_FAILURE);
}
ParseResult parse_options(char const *parse_string, size_t start,
                          size_t *new_position) {
  fprintf(stderr, "Unimplemented\n");
  exit(EXIT_FAILURE);
}
ParseResult parse_command_part(char const *parse_string, size_t start,
                               size_t *new_position) {
  ParseResult result;
  ParseResult command_name_res =
      parse_command_name(parse_string, start, new_position);

  if (command_name_res.type == ERR || command_name_res.type != COMMAND_NAME) {
    free_parse_result(command_name_res);
    result.type = ERR;
    return result;
  }

  start = *new_position;
  ParseResult options_res = parse_options(parse_string, start, new_position);

  if (options_res.type == ERR || options_res.type != OPTIONS) {
    free_parse_result(command_name_res);
    free_parse_result(options_res);
    result.type = ERR;
    return result;
  }

  result.type = COMMAND_PART;
  result.command_part.command_name = command_name_res.command_name;
  result.command_part.options = options_res.options;
  return result;
}

ParseResult parse_redirection(char const *parse_string, size_t start,
                              size_t *new_position) {
  fprintf(stderr, "Unimplemented\n");
  exit(EXIT_FAILURE);
}
ParseResult parse_command(char const *parse_string, size_t start,
                          size_t *new_position) {
  ParseResult result;
  ParseResult command_part_res =
      parse_command_part(parse_string, start, new_position);

  if (command_part_res.type == ERR || command_part_res.type != COMMAND_PART) {
    free_parse_result(command_part_res);
    result.type = ERR;
    return result;
  }

  start = *new_position;
  ParseResult redirection_res =
      parse_redirection(parse_string, start, new_position);

  if (redirection_res.type == ERR || redirection_res.type != IO_REDIRECTION) {
    free_parse_result(command_part_res);
    free_parse_result(redirection_res);
    result.type = ERR;
    return result;
  }

  result.type = COMMAND;
  result.command = create_command(command_part_res.command_part.command_name,
                                  command_part_res.command_part.options,
                                  redirection_res.redirection, false);
  return result;
}

Command *parse(char const *parse_string, size_t *num_parsed) {
  // If there is a parse error this should print invalid input and return the
  // empty commands list.
  DBG("Parsing string: %s", parse_string);

  size_t len_input = strlen(parse_string);
  if (len_input == 0) {
    *num_parsed = 0;
    return NULL;
  }

  CommandVec v;
  command_vec_init(&v);

  size_t command_end = 0;
  ParseResult parse_result;
  do {
    parse_result = parse_command(parse_string, command_end, &command_end);
    if (parse_result.type == ERR) {
      command_vec_free(&v);
      *num_parsed = 0;
      return NULL;
    } else if (parse_result.type == COMMAND) {
      command_vec_add(&v, parse_result.command);
    }
    if (command_end < len_input) {
      size_t next = command_end;
      while (next < len_input) {
        if (parse_string[next] != '&') {
          next++;
        } else if (!isspace(parse_string[next])) {
          command_vec_free(&v);
          *num_parsed = 0;
          return NULL;
        } else {
          break;
        }
      }
    }
  } while (parse_result.type != EOI);

  Command *commands = checked_calloc(*num_parsed, sizeof(*commands));

  return commands;
}
