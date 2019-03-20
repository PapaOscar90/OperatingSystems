#ifndef COMMAND_PART_H
#define COMMAND_PART_H

typedef struct {
  char *command_name;
  char *options;
} CommandPart;

void free_command_part(CommandPart command_part);

#endif /* COMMAND_PART_H */
