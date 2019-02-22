/*
 * Parsing Command Line Arguments
 *
 * Troy Karan Harrison - s2841886
 * Philip Oetinger     - s2966018
 *
 * This program reads a command (optional contained within quotes) and optional
 * arguments from stdin and runs a new process that executes the command with
 * its arguments
 */

#include "util.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#ifndef LINE_MAX
#define LINE_MAX 4096
#endif

// NOTE 127 is the exit code returned by bash when a command is not
// found on the user's path and the command is not a built-in
// shell command.
#ifndef EXIT_COMMAND_NOT_FOUND
#define EXIT_COMMAND_NOT_FOUND 127
#endif

// Parse a command from an input string.
//   A command is either the first consecutive string that contains no
//   non-line-breaking whitespace characters
//   OR
//   The text enclosed between the first pair of single or double quotes.
char *parseCommand(const char *input) {
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
                               !isNonBreakingSpace(input[offset]))) {
    if (!withinSingleQuotes && input[offset] == '"') {
      withinDoubleQuotes = !withinDoubleQuotes;
    } else if (!withinDoubleQuotes && input[offset] == '\'') {
      withinSingleQuotes = !withinSingleQuotes;
    } else {
      tempCommand[i] = input[offset];
      i++;
    }
    offset++;
  }

  if (withinDoubleQuotes || withinSingleQuotes) {
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
char *parseArguments(const char *input) {
  size_t offset = 0;
  size_t inputLen = strlen(input);

  // Skip whitespace
  while (offset < inputLen && isNonBreakingSpace(input[offset]))
    offset++;

  // Skip past the argument
  int withinDoubleQuotes = 0;
  int withinSingleQuotes = 0;
  while (offset < inputLen && (withinDoubleQuotes || withinSingleQuotes ||
                               !isNonBreakingSpace(input[offset]))) {
    if (!withinSingleQuotes && input[offset] == '"') {
      withinDoubleQuotes = !withinDoubleQuotes;
    } else if (!withinDoubleQuotes && input[offset] == '\'') {
      withinSingleQuotes = !withinSingleQuotes;
    }
    offset++;
  }

  // Skip whitespace before the arguments
  while (offset < inputLen && isNonBreakingSpace(input[offset]))
    offset++;

  if (offset < inputLen) {
    // If there are arguments, copy them
    char *arguments =
        safeCalloc(strlen(input) - offset + 1, sizeof(*arguments));
    strncpy(arguments, input + offset, strlen(input) - offset);
    return arguments;
  } else {
    // There are no arguments
    return NULL;
  }
}

// Go through the user's path from getenv to check if command exists
char *findCommandPath(char *command) {
  // Get the user's path
  char *path = getenv("PATH");

  char *absolutePath;

  struct stat buffer;

  char *token = strtok(path, ":");
  // Go through all the paths appending the command.
  //   If the path exists, then break returning that path.
  while (token != NULL) {
    absolutePath =
        safeCalloc(strlen(token) + strlen(command) + 2, sizeof(*absolutePath));
    // Append the command to the directory
    sprintf(absolutePath, "%s/%s", token, command);

    // If that path exists and is a regular file.
    if (stat(absolutePath, &buffer) == 0 && S_ISREG(buffer.st_mode)) {
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

int main() {
  // Read the commands and arguments into a string from stdin
  char *line = readLine(LINE_MAX, stdin);

  // Parse the command from the user input
  char *command = parseCommand(line);
  // Parse the arguments from the user input
  char *arguments = parseArguments(line);

  // Find the command's absolute path
  char *commandPath = findCommandPath(command);

  // Create a child process that will execute the command
  int child;
  child = fork();
  if (child < 0) {
    fprintf(stderr, "Fork failed: aborted\n");
    return EXIT_FAILURE;
  }

  if (child == 0) {
    // Child runs the new core with the command the provided arguments
    char *newargv[3];
    newargv[0] = command;
    newargv[1] = arguments;
    newargv[2] = NULL;

    execve(commandPath, newargv, NULL);

    // If the command could not be executed

    printf("Command %s not found!\n", command);

    // Cleanup
    free(line);
    free(command);
    free(arguments);
    free(commandPath);

    return EXIT_COMMAND_NOT_FOUND;
  }

  // Wait for the child to report its status
  int status;
  waitpid(-1, &status, 0);

  // Cleanup
  free(line);
  free(command);
  free(arguments);
  free(commandPath);

  // Forward the exit status of the child
  return EXIT_SUCCESS;
}
