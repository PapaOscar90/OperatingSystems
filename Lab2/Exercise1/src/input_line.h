#ifndef INPUT_LINE_H
#define INPUT_LINE_H

#include "command.h"
#include "exec.h"
#include <stdlib.h>

typedef struct {
  size_t amount;
  Command *commands;
} InputLine;

Exec exec_input_line(InputLine input_line);
void free_input_line(InputLine input_line);
InputLine new_input_line(size_t amount);

#endif /* INPUT_LINE_H */
