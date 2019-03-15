#include "shell.h"
#include "stdio.h"
#include <string.h>

// Convert a parse status to a shell status
ShellStatus shell_status_from_parse_status(ParseStatus status) {
  ShellStatus result;

  switch (status) {
  case ParseInvalidSyntaxError:
    result = ShellInvalidSyntaxError;
    break;
  case ParseOk: // TODO the shell status should only be called on errors
    result = ShellSuccessfulExecution;
    break;
  }

  return result;
}

// Convert an exec status to a shell status
ShellStatus shell_status_from_exec_status(ExecStatus status) {
  ShellStatus result;

  switch (status) {
  case ExecExit:
    result = ShellExit;
    break;
  case ExecSuccessfulExecution:
    result = ShellSuccessfulExecution;
    break;
  case ExecInputOutputToSameFileError:
    result = ShellInputOutputToSameFileError;
    break;
  case ExecExitBackgroundProcessesRunning:
    result = ShellExitBackgroundProcessesRunningError;
    break;
  case ExecCommandNotFoundError:
    result = ShellCommandNotFoundError;
    break;
  }

  return result;
}

// Display the shell prompt.
void type_prompt(void) { printf("$ "); }

// TODO this needs to be converted to an actual parse list/ AST type.
typedef int ParseList;

// Result<ParseList>
// if status is not an error, the result can be accessed.
// otherwise accessing the result is UB.
typedef struct ParseResult {
  ParseList result;
  ParseStatus status;
} ParseResult;

// Parse the provided string
ParseResult parse(char const *eval_string) {
  ParseResult result;
  if (strcmp(eval_string, "exit") == 0)
    result.status = ParseOk;
  else
    result.status = ParseInvalidSyntaxError;

  return result;
}

// Execute a parsed string
ExecStatus exec(ParseList const parsed) { return ExecExit; }

// Evaluate a string
ShellStatus eval(char const *eval_string) {
  ParseResult parsed = parse(eval_string);
  if (parsed.status < 0) {
    return shell_status_from_parse_status(parsed.status);
  }

  ExecStatus exec_status = exec(parsed.result);
  return shell_status_from_exec_status(exec_status);
}

// Pretty print a shell status
void print_shell_status(ShellStatus status) {
  switch (status) {
  case ShellCommandNotFoundError:
    printf("Error: command not found!\n");
    break;
  case ShellExit:
    printf("Shell exiting!\n");
    break;
  case ShellExitBackgroundProcessesRunningError:
    printf("There are still background processes running!\n");
    break;
  case ShellInputOutputToSameFileError:
    printf("Error: input and output files cannot be equal!\n");
    break;
  case ShellInvalidSyntaxError:
    printf("invalid syntax!\n");
    break;
  case ShellStartup:
    printf("Shell starting up\n");
    break;
  case ShellSuccessfulExecution:
    printf("Shell command successfully executed\n");
    break;
  }
}
