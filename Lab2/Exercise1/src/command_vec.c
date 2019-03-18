#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "command.h"
#include "command_vec.h"

void command_vec_init(CommandVec *v) {
  v->data = NULL;
  v->size = 0;
  v->count = 0;
}

size_t command_vec_count(CommandVec *v) { return v->count; }

void command_vec_add(CommandVec *v, Command e) {
  if (v->size == 0) {
    v->size = 10;
    v->data = malloc(sizeof(void *) * v->size);
    memset(v->data, '\0', sizeof(void *) * v->size);
  }

  if (v->size == v->count) {
    v->size *= 2;
    v->data = realloc(v->data, sizeof(void *) * v->size);
  }

  v->data[v->count] = e;
  v->count++;
}

void command_vec_set(CommandVec *v, size_t index, Command e) {
  if (index >= v->count) {
    return;
  }

  v->data[index] = e;
}

Command *command_vec_get(CommandVec *v, size_t index) {
  if (index >= v->count) {
    return NULL;
  }

  return &v->data[index];
}

void command_vec_delete(CommandVec *v, size_t index) {
  if (index >= v->count) {
    return;
  }

  free_command(v->data[index]);

  for (size_t i = index, j = index; i < v->count; i++) {
    v->data[j] = v->data[i];
    j++;
  }

  v->count--;
}

void command_vec_free(CommandVec *v) {
  for (size_t i = 0; i < v->count; ++i) {
    free_command(v->data[i]);
  }
  free(v->data);
}
