#include "macros.h"

#include "checked.h"
#include "command.h"
#include "exec.h"
#include "util.h"
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

void free_command(Command command) {
  free_command_list(command.command_list);
  free_io_redirection(command.io_redirection);
}

Exec exec_command(Command command, bool in_background) {
  if (strcmp(command.command_list.command_parts[0].command_name, "exit") == 0) {
    Exec result = {.type = EXEC_EXIT};
    return result;
  }

  // Execute the command.
  pid_t child = checked_fork();

  if (child != 0) {
    // Parent

    // If the process is in the foreground, wait for the child;
    if (!in_background) {
      waitpid(child, NULL, 0);
    }
  } else {
    // Child
    DBG("Executing COMMAND:");
    Exec exec = exec_command_list(command.command_list, command.io_redirection);

    switch (exec.type) {
    case EXEC_ERR:
      exit(EXIT_FAILURE);
    case EXEC_COMMAND:
      exit(EXIT_SUCCESS);
    case EXEC_EXIT:
      UNREACHABLE("Parent handles exiting.");
    }
  }

  Exec result = {.type = EXEC_COMMAND};
  return result;
}
