// Adapted from
// https://gist.githubusercontent.com/EmilHernvall/953968/raw/a68b896bf9ce61190a3f93ff4c51c19af0702a5f/vector.h

#ifndef VECTOR_H__
#define VECTOR_H__
#include "command.h"
#include <stdlib.h>

typedef struct CommandVec {
  Command *data;
  size_t size;
  size_t count;
} CommandVec;

void command_vec_init(CommandVec *);
size_t command_vec_count(CommandVec *);
void command_vec_add(CommandVec *, Command);
void command_vec_set(CommandVec *, size_t, Command);
Command *command_vec_get(CommandVec *, size_t);
void command_vec_delete(CommandVec *, size_t);
void command_vec_free(CommandVec *);

#endif
