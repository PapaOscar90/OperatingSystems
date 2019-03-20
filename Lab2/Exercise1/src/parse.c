#include "macros.h"

#include "checked.h"
#include "parse.h"
#include "util.h"
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

/// Return whether a character is quote or not (' or ").
bool is_quote(char const c) { return c == '\'' || c == '"'; }

/// A check for symbols that mark the bound of a command.
bool is_command_name_bound_symbol(char const c) { return c == '&' || c == '|'; }

/// A check for symbols that mark the bound of a filename.
bool is_file_name_bound_symbol(char c) {
  return c == '<' || c == '>' || is_command_name_bound_symbol(c);
}

/// A check for the symbols that mark the bound of options.
bool is_options_bound_symbol(char const c) {
  return is_file_name_bound_symbol(c);
}

char *strip_quotes(char const *string) {
  // Allocate maximum possible result size, the length of the original string.
  char *intermediary = checked_malloc(strlen(string) + 1);
  size_t result_len = 0;

  bool in_quote = false;
  char quote = '\0';

  for (size_t i = 0; i < strlen(string); ++i) {
    if (in_quote && string[i] == quote) {
      in_quote = false;
      quote = '\0';
      continue;
    } else if (!in_quote && is_quote(string[i])) {
      in_quote = true;
      quote = string[i];
      continue;
    }

    intermediary[result_len++] = string[i];
  }

  char *result = checked_malloc(result_len);
  strncpy(result, intermediary, result_len);
  result[result_len] = '\0';

  free(intermediary);

  if (in_quote) {
    free(result);
    return NULL;
  } else {
    return result;
  }
}

void free_parse(Parse parse) {
  switch (parse.type) {
  case PARSE_ERR:
    break;
  case PARSE_INPUT_LINE:
    free_input_line(parse.input_line);
    break;
  case PARSE_COMMAND:
    free_command(parse.command);
    break;
  case PARSE_IO_REDIRECTION:
    free_io_redirection(parse.io_redirection);
    break;
  case PARSE_COMMAND_LIST:
    free_command_list(parse.command_list);
    break;
  case PARSE_COMMAND_PART:
    free_command_part(parse.command_part);
    break;
  case PARSE_FILE_NAME:
    free(parse.file_name);
    break;
  case PARSE_COMMAND_NAME:
    free(parse.command_name);
    break;
  case PARSE_OPTIONS:
    free(parse.options);
    break;
  }
}

Parse parse(char const *input) {
  size_t position = 0;
  Parse parsed = parse_input_line(input, position, &position);

  assert(parsed.type == PARSE_ERR || parsed.type == PARSE_INPUT_LINE);

  DBG("%lu %lu %d", position, strlen(input), parsed.type == PARSE_ERR);
  // Not all the characters were processed.
  if (position < strlen(input)) {
    free_parse(parsed);
    Parse err = {.type = PARSE_ERR};
    return err;
  }

  return parsed;
}

void skip_whitespace(char const *input, size_t start, size_t *new_position) {
  // If the provided start position is outside of bounds, do nothing.
  size_t len = strlen(input);
  if (start >= len) {
    return;
  }

  // From the start position onwards, check each character
  for (size_t i = start; i < len; ++i) {
    // If the character is not a whitespace character
    if (!isspace(input[i])) {
      // Assign the current position as the new_position and return
      *new_position = i;
      return;
    }
  }

  // The rest of the string is solely whitespace
  // Set the position to the end of the string.
  *new_position = len;
}

size_t number_of_commands(size_t start, char const *input) {
  size_t count = 1;
  bool in_quote = false;
  char quote = '\0';

  for (size_t i = start; i < strlen(input); ++i) {
    if (!in_quote && is_quote(input[i])) {
      in_quote = true;
      quote = input[i];
      continue;
    }
    if (in_quote && input[i] == quote) {
      in_quote = false;
      quote = '\0';
      continue;
    }

    if (!in_quote && input[i] == '&') {
      count++;
    }
  }

  return count;
}

size_t number_of_command_parts(size_t start, char const *input) {
  size_t count = 1;
  bool in_quote = false;
  char quote = '\0';

  for (size_t i = start; i < strlen(input); ++i) {
    if (!in_quote && is_quote(input[i])) {
      in_quote = true;
      quote = input[i];
      continue;
    }
    if (in_quote && input[i] == quote) {
      in_quote = false;
      quote = '\0';
      continue;
    }

    if (!in_quote) {
      if (input[i] == '&')
        break;
      else if (input[i] == '|')
        count++;
    }
  }

  return count;
}

Parse parse_input_line(char const *input, size_t start, size_t *new_position) {
  InputLine input_line = new_input_line(number_of_commands(start, input));

  *new_position = start;
  for (size_t i = 0; i < input_line.amount; ++i) {
    Parse command_parse = parse_command(input, *new_position, new_position);

    if (command_parse.type == PARSE_ERR) {
      free_parse(command_parse);
      input_line.amount = i;
      free_input_line(input_line);
      Parse err = {.type = PARSE_ERR};
      return err;
    }

    assert(command_parse.type == PARSE_COMMAND);

    input_line.commands[i] = command_parse.command;

    if (i < input_line.amount - 1) {
      skip_whitespace(input, *new_position, new_position);
      if (input[*new_position] != '&') {
        free_parse(command_parse);
        input_line.amount = i;
        free_input_line(input_line);
        Parse err = {.type = PARSE_ERR};
        return err;
      }
      (*new_position)++;
      skip_whitespace(input, *new_position, new_position);
      DBG("%lu", *new_position);
    }
  }

  Parse result = {.type = PARSE_INPUT_LINE, .input_line = input_line};
  return result;
}

Parse parse_command(char const *input, size_t start, size_t *new_position) {
  Parse command_list_parse = parse_command_list(input, start, new_position);

  if (command_list_parse.type == PARSE_ERR) {
    free_parse(command_list_parse);
    Parse err = {.type = PARSE_ERR};
    return err;
  }

  assert(command_list_parse.type == PARSE_COMMAND_LIST);

  skip_whitespace(input, *new_position, new_position);

  Parse io_redirection_parse =
      parse_io_redirection(input, *new_position, new_position);

  if (io_redirection_parse.type == PARSE_ERR) {
    free_parse(command_list_parse);
    free_parse(io_redirection_parse);
    Parse err = {.type = PARSE_ERR};
    return err;
  }

  assert(io_redirection_parse.type == PARSE_IO_REDIRECTION);

  Command command = {.command_list = command_list_parse.command_list,
                     .io_redirection = io_redirection_parse.io_redirection};

  Parse result = {.type = PARSE_COMMAND, .command = command};
  return result;
}

Parse parse_io_redirection(char const *input, size_t start,
                           size_t *new_position) {
  IoRedirection io_redirection = {.type = IO_REDIRECTION_EMPTY};
  if (input[start] == '<') {
    (*new_position)++;
    skip_whitespace(input, *new_position, new_position);
    Parse filename_1_parse =
        parse_file_name(input, *new_position, new_position);
    if (filename_1_parse.type == PARSE_ERR) {
      free_parse(filename_1_parse);
      free_io_redirection(io_redirection);
      Parse err = {.type = PARSE_ERR};
      return err;
    }
    assert(filename_1_parse.type == PARSE_FILE_NAME);

    skip_whitespace(input, *new_position, new_position);

    if (input[*new_position] == '>') {
      (*new_position)++;
      Parse filename_2_parse =
          parse_file_name(input, *new_position, new_position);
      if (filename_2_parse.type == PARSE_ERR) {
        free_parse(filename_1_parse);
        free_parse(filename_2_parse);
        free_io_redirection(io_redirection);
        Parse err = {.type = PARSE_ERR};
        return err;
      }

      assert(filename_2_parse.type == PARSE_FILE_NAME);

      io_redirection.type = IO_REDIRECTION_LEFT_RIGHT;
      io_redirection.file1 = filename_1_parse.file_name;
      io_redirection.file2 = filename_2_parse.file_name;
    } else {
      io_redirection.type = IO_REDIRECTION_LEFT;
      io_redirection.file = filename_1_parse.file_name;
    }

  } else if (input[start] == '>') {
    (*new_position)++;

    skip_whitespace(input, *new_position, new_position);
    Parse filename_1_parse =
        parse_file_name(input, *new_position, new_position);
    if (filename_1_parse.type == PARSE_ERR) {
      free_parse(filename_1_parse);
      free_io_redirection(io_redirection);
      Parse err = {.type = PARSE_ERR};
      return err;
    }
    assert(filename_1_parse.type == PARSE_FILE_NAME);

    skip_whitespace(input, *new_position, new_position);
    if (input[*new_position] == '<') {
      (*new_position)++;
      Parse filename_2_parse =
          parse_file_name(input, *new_position, new_position);
      if (filename_2_parse.type == PARSE_ERR) {
        free_parse(filename_1_parse);
        free_parse(filename_2_parse);
        free_io_redirection(io_redirection);
        Parse err = {.type = PARSE_ERR};
        return err;
      }

      assert(filename_2_parse.type == PARSE_FILE_NAME);

      io_redirection.type = IO_REDIRECTION_RIGHT_LEFT;
      io_redirection.file1 = filename_1_parse.file_name;
      io_redirection.file2 = filename_2_parse.file_name;
    } else {
      io_redirection.type = IO_REDIRECTION_RIGHT;
      io_redirection.file = filename_1_parse.file_name;
    }
  } else {
    io_redirection.type = IO_REDIRECTION_EMPTY;
  }

  Parse result = {.type = PARSE_IO_REDIRECTION,
                  .io_redirection = io_redirection};
  return result;
}

Parse parse_command_list(char const *input, size_t start,
                         size_t *new_position) {
  CommandList command_list =
      new_command_list(number_of_command_parts(start, input));

  *new_position = start;

  for (size_t i = 0; i < command_list.amount; ++i) {
    Parse command_part_parse =
        parse_command_part(input, *new_position, new_position);
    if (command_part_parse.type == PARSE_ERR) {
      free_parse(command_part_parse);
      command_list.amount = i;
      free_command_list(command_list);
      Parse err = {.type = PARSE_ERR};
      return err;
    }

    assert(command_part_parse.type == PARSE_COMMAND_PART);

    command_list.command_parts[i] = command_part_parse.command_part;

    if (i < command_list.amount - 1) {
      skip_whitespace(input, *new_position, new_position);
      if (input[*new_position] != '|') {
        free_parse(command_part_parse);
        command_list.amount = i;
        free_command_list(command_list);
        Parse err = {.type = PARSE_ERR};
        return err;
      }
      (*new_position)++;
      skip_whitespace(input, *new_position, new_position);
    }
  }

  Parse result = {.type = PARSE_COMMAND_LIST, .command_list = command_list};
  return result;
}

Parse parse_command_part(char const *input, size_t start,
                         size_t *new_position) {
  Parse command_name_parse = parse_command_name(input, start, new_position);

  if (command_name_parse.type == PARSE_ERR) {
    free_parse(command_name_parse);
    Parse err = {.type = PARSE_ERR};
    return err;
  }

  assert(command_name_parse.type == PARSE_COMMAND_NAME);

  skip_whitespace(input, *new_position, new_position);

  Parse options_parse = parse_options(input, *new_position, new_position);

  if (options_parse.type == PARSE_ERR) {
    free_parse(command_name_parse);
    free_parse(options_parse);
    Parse err = {.type = PARSE_ERR};
    return err;
  }

  assert(options_parse.type == PARSE_OPTIONS);

  CommandPart command_part = {.command_name = command_name_parse.command_name,
                              .options = options_parse.options};

  Parse result = {.type = PARSE_COMMAND_PART, .command_part = command_part};
  return result;
}

Parse parse_command_name(char const *input, size_t start,
                         size_t *new_position) {
  size_t len = strlen(input);

  Parse result;
  skip_whitespace(input, start, &start);
  if (start >= len) {
    result.type = PARSE_ERR;
    return result;
  }

  bool in_quote = false;
  char quote = '\0';

  char *intermediary = checked_malloc(len + 1);
  size_t amount = 0;

  size_t i;
  for (i = start; i < len; ++i) {
    if (in_quote && quote == input[i]) {
      intermediary[amount++] = input[i];
      in_quote = false;
      quote = '\0';
      continue;
    }

    if (!in_quote && is_quote(input[i])) {
      intermediary[amount++] = input[i];
      in_quote = true;
      quote = input[i];
      continue;
    }

    if (in_quote) {
      intermediary[amount++] = input[i];
      continue;
    }

    if (!in_quote && isspace(input[i])) {
      break;
    }

    if (!in_quote && is_command_name_bound_symbol(input[i])) {
      break;
    }

    if (!in_quote) {
      intermediary[amount++] = input[i];
      continue;
    }
  }

  start = i;

  if (in_quote) {
    free(intermediary);
    result.type = PARSE_ERR;
    return result;
  }

  char *command_name = checked_malloc(amount + 1);
  strncpy(command_name, intermediary, amount);
  command_name[amount] = '\0';

  free(intermediary);

  *new_position = start;
  result.type = PARSE_COMMAND_NAME;
  result.command_name = command_name;

  return result;
}

Parse parse_options(char const *input, size_t start, size_t *new_position);

Parse parse_options(char const *input, size_t start, size_t *new_position) {

  size_t len = strlen(input);

  Parse result;
  skip_whitespace(input, start, &start);
  if (start >= len) {
    *new_position = len;
    result.type = PARSE_OPTIONS;
    result.options = NULL;
    return result;
  }

  bool in_quote = false;
  char quote = '\0';

  char *intermediary = checked_malloc(len + 1);
  size_t amount = 0;

  size_t i;
  for (i = start; i < len; ++i) {
    if (in_quote && quote == input[i]) {
      in_quote = false;
      quote = '\0';
      continue;
    }

    if (!in_quote && is_quote(input[i])) {
      in_quote = true;
      quote = input[i];
      continue;
    }

    if (in_quote) {
      intermediary[amount++] = input[i];
      continue;
    }

    if (!in_quote && is_options_bound_symbol(input[i])) {
      break;
    }

    if (!in_quote) {
      intermediary[amount++] = input[i];
      continue;
    }
  }

  start = i;

  if (in_quote) {
    free(intermediary);
    result.type = PARSE_ERR;
    return result;
  }

  char *options = checked_malloc(amount + 1);
  strncpy(options, intermediary, amount);
  options[amount] = '\0';

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

  result.type = PARSE_OPTIONS;
  if (strlen(options) == 0) {
    free(options);
    result.options = NULL;
  } else {
    result.options = options;
  }

  return result;
}

Parse parse_file_name(char const *input, size_t start, size_t *new_position) {
  size_t len = strlen(input);

  Parse result;
  skip_whitespace(input, start, &start);
  if (start >= len) {
    result.type = PARSE_ERR;
    return result;
  }

  bool in_quote = false;
  char quote = '\0';

  char *intermediary = checked_malloc(len + 1);
  size_t amount = 0;

  size_t i;
  for (i = start; i < len; ++i) {
    if (in_quote && quote == input[i]) {
      intermediary[amount++] = input[i];
      in_quote = false;
      quote = '\0';
      continue;
    }

    if (!in_quote && is_quote(input[i])) {
      intermediary[amount++] = input[i];
      in_quote = true;
      quote = input[i];
      continue;
    }

    if (in_quote) {
      intermediary[amount++] = input[i];
      continue;
    }

    if (!in_quote && isspace(input[i])) {
      break;
    }

    if (!in_quote && is_file_name_bound_symbol(input[i])) {
      break;
    }

    if (!in_quote) {
      intermediary[amount++] = input[i];
      continue;
    }
  }

  start = i;

  if (in_quote) {
    free(intermediary);
    result.type = PARSE_ERR;
    return result;
  }

  char *file_name = checked_malloc(amount + 1);
  strncpy(file_name, intermediary, amount);
  file_name[amount] = '\0';

  free(intermediary);

  *new_position = start;
  result.type = PARSE_FILE_NAME;
  result.file_name = file_name;

  return result;
}
