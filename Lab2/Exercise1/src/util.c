#include "util.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *checked_malloc(size_t size)
{
  void *p = malloc(size);
  if (p == NULL)
  {
    fprintf(stderr, "Malloc failed, out of memory?\n");
    exit(EXIT_FAILURE);
  }

  return p;
}

void *checked_calloc(size_t nmemb, size_t size)
{
  void *p = calloc(nmemb, size);
  if (p == NULL)
  {
    fprintf(stderr, "Calloc failed, out of memory?\n");
    exit(EXIT_FAILURE);
  }

  return p;
}

void read_line(char **line, size_t *line_len)
{
  getline(line, line_len, stdin);
  *line_len = strlen(*line) - 1;
  (*line)[*line_len] = '\0';
}

char *unquote_string(char const *string)
{
  char *intermediary = checked_malloc(strlen(string) + 1);
  size_t result_len = 0;
  bool in_quote = false;
  char quote = '\0';

  for (size_t i = 0; i < strlen(string); ++i)
  {
    if (in_quote && string[i] == quote)
    {
      in_quote = false;
      quote = '\0';
      continue;
    }
    else if (!in_quote && (string[i] == '\'' || string[i] == '"'))
    {
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

  if (in_quote)
  {
    free(result);
    return NULL;
  }
  else
  {
    return result;
  }
}

// Parse a command from an input string.
//   A command is either the first consecutive string that contains no
//   non-line-breaking whitespace characters
//   OR
//   The text enclosed between the first pair of single or double quotes.
char *parseCommand(const char *input)
{
  size_t offset = 0;
  size_t inputLen = strlen(input);

  char *tempCommand = safeCalloc(inputLen + 1, sizeof(*tempCommand));

  // Skip whitespace
  while (offset < inputLen && isNonBreakingSpace(input[offset]))
    offset++;

  int withinDoubleQuotes = 0;
  int withinSingleQuotes = 0;
  size_t i = 0;
  while (offset < inputLen && (withinDoubleQuotes || withinSingleQuotes ||
                               !isNonBreakingSpace(input[offset])))
  {
    if (!withinSingleQuotes && input[offset] == '"')
    {
      withinDoubleQuotes = !withinDoubleQuotes;
    }
    else if (!withinDoubleQuotes && input[offset] == '\'')
    {
      withinSingleQuotes = !withinSingleQuotes;
    }
    else
    {
      tempCommand[i] = input[offset];
      i++;
    }
    offset++;
  }

  if (withinDoubleQuotes || withinSingleQuotes)
  {
    free(tempCommand);
    fprintf(stderr, "Mismatched quotation: %c\n",
            withinDoubleQuotes ? '"' : '\'');
    exit(EXIT_FAILURE);
  }

  tempCommand[offset] = '\0';

  // Copy the command from the temporary buffer and return it
  char *command = safeCalloc(strlen(tempCommand) + 1, sizeof(*command));
  strcpy(command, tempCommand);
  free(tempCommand);
  return command;
}

// Extract the arguments from an input string.
char *parseArguments(const char *input)
{
  size_t offset = 0;
  size_t inputLen = strlen(input);

  // Skip whitespace
  while (offset < inputLen && isNonBreakingSpace(input[offset]))
    offset++;

  // Skip past the argument
  int withinDoubleQuotes = 0;
  int withinSingleQuotes = 0;
  while (offset < inputLen && (withinDoubleQuotes || withinSingleQuotes ||
                               !isNonBreakingSpace(input[offset])))
  {
    if (!withinSingleQuotes && input[offset] == '"')
    {
      withinDoubleQuotes = !withinDoubleQuotes;
    }
    else if (!withinDoubleQuotes && input[offset] == '\'')
    {
      withinSingleQuotes = !withinSingleQuotes;
    }
    offset++;
  }

  // Skip whitespace before the arguments
  while (offset < inputLen && isNonBreakingSpace(input[offset]))
    offset++;

  if (offset < inputLen)
  {
    // If there are arguments, copy them
    char *arguments =
        safeCalloc(strlen(input) - offset + 1, sizeof(*arguments));
    strncpy(arguments, input + offset, strlen(input) - offset);
    return arguments;
  }
  else
  {
    // There are no arguments
    return NULL;
  }
}

// Go through the user's path from getenv to check if command exists
char *findCommandPath(char *command)
{
  // Get the user's path
  char *path = getenv("PATH");

  char *absolutePath;

  struct stat buffer;

  char *token = strtok(path, ":");
  // Go through all the paths appending the command.
  //   If the path exists, then break returning that path.
  while (token != NULL)
  {
    absolutePath =
        safeCalloc(strlen(token) + strlen(command) + 2, sizeof(*absolutePath));
    // Append the command to the directory
    sprintf(absolutePath, "%s/%s", token, command);

    // If that path exists and is a regular file.
    if (stat(absolutePath, &buffer) == 0 && S_ISREG(buffer.st_mode))
    {
      return absolutePath;
    }

    // Get the next directory from the path
    token = strtok(NULL, ":");
    // Cleanup
    free(absolutePath);
  }

  // The command is not in the user's path
  return NULL;
}
