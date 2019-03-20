#include "input_line.h"
#include "checked.h"
#include "command.h"
#include <stdlib.h>

Exec exec_input_line(InputLine input_line) {
  // For each command in input_line
  for (size_t i = 0; i < input_line.amount; ++i) {

    // All commands should be executed in the background save for the last one.
    bool in_background = i < input_line.amount - 1;

    // Execute that command
    Exec executed = exec_command(input_line.commands[i], in_background);

    switch (executed.type) {
    case EXEC_ERR:
      // If there was an error in execution.
      return executed;
    case EXEC_EXIT:
      // If the command was to exit.
      return executed;
    case EXEC_COMMAND:
      // Nothing needs to be done in this case.
      break;
    }
    free_exec(executed);
  }

  Exec result = {.type = EXEC_COMMAND};
  return result;
}

void free_input_line(InputLine input_line) {
  // For each command, free it.
  for (size_t i = 0; i < input_line.amount; ++i) {
    free_command(input_line.commands[i]);
  }
  free(input_line.commands);
}

InputLine new_input_line(size_t amount) {
  InputLine input_line = {.amount = amount,
                          .commands = checked_calloc(amount, sizeof(Command))};
  return input_line;
}
