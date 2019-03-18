#include "redirection.h"
#include <stdlib.h>

void free_redirection(Redirection redirection) {
  switch (redirection.type) {
  case LEFT_RIGHT: // fallthrough
  case RIGHT_LEFT:
    free(redirection.left_file);
    free(redirection.right_file);
    break;
  case LEFT: // fallthrough
  case RIGHT:
    free(redirection.file);
    break;
  case NONE: // Do nothing
    break;
  }
}

Redirection create_redirection(RedirectionType type, char *file1, char *file2) {
  Redirection redirection;
  redirection.type = type;

  switch (redirection.type) {

  case LEFT_RIGHT:
  case RIGHT_LEFT:
    redirection.left_file = file1;
    redirection.right_file = file2;
    break;

  case LEFT:
  case RIGHT:
    redirection.file = file1;
    break;

  case NONE:
    break;
  }

  return redirection;
}
