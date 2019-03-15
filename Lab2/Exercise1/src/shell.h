#ifndef SHELL_H
#define SHELL_H

#include "stdio.h"

// Possible states for the shell to be in
// Negative values represent errors
typedef enum ShellStatus {
  // Initial state for the shell
  ShellStartup = 0,

  // Exit the shell
  ShellExit = 1,

  // Execution of the previous command was successful (typically no response
  // should be necessary)
  ShellSuccessfulExecution = 2,

  // The previous command had invalid syntax
  ShellInvalidSyntaxError = -1,

  // The previous command involved input and output to the same file
  ShellInputOutputToSameFileError = -2,

  // The previous command was to exit while background processes were still
  // running
  ShellExitBackgroundProcessesRunningError = -3,

  // The previous command was not a shell builtin nor on the path
  ShellCommandNotFoundError = -4
} ShellStatus;

// Possible states for a parse to be in
// Negative values represent errors
typedef enum ParseStatus {
  ParseOk = 0,
  ParseInvalidSyntaxError = -1,
} ParseStatus;

// Possible states for an execution to be in
// Negative values represent errors
typedef enum ExecStatus {
  ExecExit = 0,
  ExecSuccessfulExecution = 1,
  ExecInputOutputToSameFileError = -1,
  ExecExitBackgroundProcessesRunning = -2,
  ExecCommandNotFoundError = -3,
} ExecStatus;

// Pretty print a shell status
void print_shell_status(ShellStatus status);

// Convert a parse status to a shell status
ShellStatus shell_status_from_parse_status(ParseStatus status);

// Convert an exec status to a shell status
ShellStatus shell_status_from_exec_status(ExecStatus status);

// Display the shell prompt.
void type_prompt(void);

// Evaluate a string
ShellStatus eval(char const *eval_string);

#endif /* SHELL_H */
