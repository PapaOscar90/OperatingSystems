#define DEBUG 1

#include "macros.h"

#include "command.h"
#include "command_vec.h"
#include "parse.h"
#include "redirection.h"
#include "util.h"
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

void print_parse_result(ParseResult result) {
  switch (result.type) {
  case ERR:
    printf("ERR\n");
    break;
  case EOI:
    printf("EOI\n");
    break;
  case COMMAND_PART:
    printf("COMMAND_PART:\n\tcommand: '%s'\n\toptions:'%s'\n",
           result.command_part.command_name, result.command_part.options);
  case COMMAND_NAME:
    printf("COMMAND_NAME: '%s'\n", result.command_name);
    break;
  case OPTIONS:
    printf("OPTIONS: '%s'\n", result.command_name);
    free(result.options);
    break;
  case IO_REDIRECTION:
    printf("IO_REDIRECTION: ");
    /* print_redirection(result.redirection); */
    putchar('\n');
    break;
  case COMMAND:
    print_command(result.command);
    putchar('\n');
    break;
  case INPUT_LINE:
    printf("INPUT_LINE: ");
    /* print_commands(result.commands); // needs number of commands as well.*/
    putchar('\n');
  }
}

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

bool is_special_shell_symbol(char const c) { return c == '&'; }

void skip_whitespace(char const *parse_string, size_t start,
                     size_t *new_position) {
  // If the provided start position is outside of bounds, do nothing.
  size_t len = strlen(parse_string);
  if (start >= len) {
    return;
  }

  // From the start position onwards, check each character
  for (size_t i = start; i < len; ++i) {
    // If the character is not a whitespace character
    if (!isspace(parse_string[i])) {
      // Assign the current position as the new_position and return
      *new_position = i;
      return;
    }
  }

  // The rest of the string is solely whitespace
  // Set the position to the end of the string.
  *new_position = len;
}

bool is_quote(char const c) { return c == '"' || c == '\''; }

ParseResult parse_command_name(char const *parse_string, size_t start,
                               size_t *new_position) {
  size_t len = strlen(parse_string);

  ParseResult result;
  skip_whitespace(parse_string, start, &start);
  if (start >= len) {
    result.type = ERR;
    return result;
  }

  bool in_quote = false;
  char quote = '\0';

  char *intermediary = checked_malloc(len + 1);
  size_t amount = 0;

  size_t i;
  for (i = start; i < len; ++i) {
    if (in_quote && quote == parse_string[i]) {
      in_quote = false;
      quote = '\0';
      continue;
    }

    if (!in_quote && is_quote(parse_string[i])) {
      in_quote = true;
      quote = parse_string[i];
      continue;
    }

    if (in_quote) {
      intermediary[amount++] = parse_string[i];
      continue;
    }

    if (!in_quote && isspace(parse_string[i])) {
      break;
    }

    if (!in_quote && is_special_shell_symbol(parse_string[i])) {
      break;
    }

    if (!in_quote) {
      intermediary[amount++] = parse_string[i];
      continue;
    }
  }

  start = i;

  if (in_quote) {
    free(intermediary);
    result.type = ERR;
    return result;
  }

  char *intermediary2 = checked_malloc(amount + 1);
  strncpy(intermediary2, intermediary, amount);
  intermediary2[amount] = '\0';

  char *command_name = unquote_string(intermediary2);

  free(intermediary2);
  free(intermediary);

  *new_position = start;
  result.type = COMMAND_NAME;
  result.command_name = command_name;

  return result;
}

ParseResult parse_options(char const *parse_string, size_t start,
                          size_t *new_position) {
  size_t len = strlen(parse_string);

  ParseResult result;
  skip_whitespace(parse_string, start, &start);
  if (start >= len) {
    *new_position = len;
    result.type = OPTIONS;
    result.options = NULL;
    return result;
  }

  bool in_quote = false;
  char quote = '\0';

  char *intermediary = checked_malloc(len + 1);
  size_t amount = 0;

  size_t i;
  for (i = start; i < len; ++i) {
    if (in_quote && quote == parse_string[i]) {
      in_quote = false;
      quote = '\0';
      continue;
    }

    if (!in_quote && is_quote(parse_string[i])) {
      in_quote = true;
      quote = parse_string[i];
      continue;
    }

    if (in_quote) {
      intermediary[amount++] = parse_string[i];
      continue;
    }

    if (!in_quote && is_special_shell_symbol(parse_string[i])) {
      break;
    }

    if (!in_quote) {
      intermediary[amount++] = parse_string[i];
      continue;
    }
  }

  start = i;

  if (in_quote) {
    free(intermediary);
    result.type = ERR;
    return result;
  }

  char *intermediary2 = checked_malloc(amount + 1);
  strncpy(intermediary2, intermediary, amount);
  intermediary2[amount] = '\0';

  char *options = unquote_string(intermediary2);

  free(intermediary2);
  free(intermediary);

  *new_position = start;

  // Trim trailing whitespace
  long trail = strlen(options) - 1;
  while (trail >= 0) {
    if (!isspace(options[trail])) {
      options[trail + 1] = '\0';
      break;
    }
    trail--;
  }

  if (strlen(options) == 0) {
    free(options);
    result.type = OPTIONS;
    result.options = NULL;
  } else {
    result.type = OPTIONS;
    result.options = options;
  }

  return result;
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
  if (start >= strlen(parse_string)) {
    result.type = EOI;
    return result;
  }

  ParseResult command_part_res =
      parse_command_part(parse_string, start, new_position);

  if (command_part_res.type == ERR || command_part_res.type != COMMAND_PART) {
    free_parse_result(command_part_res);
    result.type = ERR;
    return result;
  }

  start = *new_position;
  // TODO
  ParseResult redirection_res = {.type = IO_REDIRECTION,
                                 .redirection = {.type = NONE}};
  /* parse_redirection(parse_string, start, new_position); */

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

  size_t len = strlen(parse_string);
  if (len == 0) {
    *num_parsed = 0;
    return NULL;
  }

  CommandVec v;
  command_vec_init(&v);

  size_t position = 0;

  ParseResult parsed_command;
  do {
    parsed_command = parse_command(parse_string, position, &position);
    if (parsed_command.type == ERR) {
      command_vec_free(&v);
      *num_parsed = 0;
      return NULL;
    }

    if (parsed_command.type == COMMAND) {

      if (position < len) {
        skip_whitespace(parse_string, position, &position);
        if (parse_string[position] == '&') {
          position++;
          parsed_command.command.in_background = true;
        } else if (position >= len) {
          command_vec_free(&v);
          *num_parsed = 0;
          return NULL;
        }
      }
      command_vec_add(&v, parsed_command.command);
    }
  } while (parsed_command.type != EOI);

  (*num_parsed) = command_vec_count(&v);
  Command *commands = checked_calloc(*num_parsed, sizeof(*commands));
  for (size_t i = 0; i < *num_parsed; ++i) {
    commands[i] = *command_vec_get(&v, i);
  }

  return commands;
}
