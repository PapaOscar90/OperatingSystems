#include "macros.h"

#include "exec.h"
#include "input_line.h"
#include "parse.h"
#include "shell.h"
#include <assert.h>
#include <stdio.h>

/// Evaluate the provided string, thereby executing the encoded commands.
Exec eval(char const *input) {
  // Attempt to parse the input.
  Parse parsed = parse(input);

  // If there was an error in parsing.
  if (parsed.type == PARSE_ERR) {
    // Cleanup
    printf("Invalid syntax!\n");
    free_parse(parsed);
    Exec result = {.type = EXEC_ERR};
    return result;
  }

  // Assert an invariant on the parse status type.
  assert(parsed.type == PARSE_INPUT_LINE);

  // For each command in the input line
  Exec exec = exec_input_line(parsed.input_line);

  // Cleanup
  free_parse(parsed);
  return exec;
}

/// Display a prompt to the user.
void type_prompt(void) {
  // No prompt is required for the Themis submission
  /* printf("$ "); */
}
