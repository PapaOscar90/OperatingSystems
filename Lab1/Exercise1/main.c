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
// shell command
#ifndef EXIT_COMMAND_NOT_FOUND
#define EXIT_COMMAND_NOT_FOUND 127
#endif

// Parse a command from an input string.
//   A command is either the first consecutive string that contains no
//   non-line-breaking whitespace characters
//   OR
//   The text enclosed between the first pair of single or double quotes.
char *parseCommand(const char *input) {
  size_t i = 0;
  size_t inputLen = strlen(input);
  size_t startOfCommand;
  size_t endOfCommand;
  // Skip whitespace
  while (i < inputLen && isNonBreakingSpace(input[i]))
    i++;

  // If the command starts with a double quote, search for the closing quote
  if (input[i] == '"' || input[i] == '\'') {
    size_t firstQuoteIdx = i;
    size_t secondQuoteIdx = i + 1;
    // Find the position of the second quote
    while (secondQuoteIdx < inputLen &&
           input[secondQuoteIdx] != input[firstQuoteIdx]) {
      secondQuoteIdx++;
    }

    // If no matching quote was found
    if (secondQuoteIdx >= inputLen) {
      fprintf(stderr, "Mismatched quotation: %c\n", input[firstQuoteIdx]);
      exit(EXIT_FAILURE);
    }

    // Enclosed is the command sans the quotes
    startOfCommand = firstQuoteIdx + 1;
    endOfCommand = secondQuoteIdx;
  } else {
    // As there are no quotes the end of the command is denoted by the first
    // whitespace character
    startOfCommand = i;
    endOfCommand = i + 1;

    // Increment till a whitespace character is found
    while (endOfCommand < inputLen &&
           !isNonBreakingSpace(input[endOfCommand])) {
      endOfCommand++;
    }
  }

  // Copy the command from the line and return it
  char *command =
      safeCalloc(endOfCommand - startOfCommand + 1, sizeof(*command));
  strncpy(command, input + startOfCommand, endOfCommand - startOfCommand);
  return command;
}

// Extract the arguments from an input string.
char *parseArguments(const char *input) {
  size_t i = 0;
  size_t inputLen = strlen(input);

  // Skip whitespace
  while (i < inputLen && isNonBreakingSpace(input[i]))
    i++;

  // Skip past the argument
  if (input[i] == '"' || input[i] == '\'') {
    // Skip past the quotes if they exist
    size_t firstQuoteIdx = i;
    i++;
    while (i < inputLen && input[i] != input[firstQuoteIdx])
      i++;
  } else {
    // Skip past the text of the argument
    while (i < inputLen && !isNonBreakingSpace(input[i]))
      i++;
  }

  // Skip whitespace before the arguments
  while (i < inputLen && isNonBreakingSpace(input[i]))
    i++;

  if (i < inputLen) {
    // If there are arguments, copy them
    char *arguments = safeCalloc(strlen(input) - i + 1, sizeof(*arguments));
    strncpy(arguments, input + i, strlen(input) - i);
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
  return WEXITSTATUS(status);
}
