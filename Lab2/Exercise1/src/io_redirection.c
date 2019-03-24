#include "io_redirection.h"
#include <stdlib.h>

void free_io_redirection(IoRedirection io_redirection) {
  switch (io_redirection.type) {

  case IO_REDIRECTION_LEFT_RIGHT:
  case IO_REDIRECTION_RIGHT_LEFT:
    free(io_redirection.file1);
    free(io_redirection.file2);
    break;

  case IO_REDIRECTION_RIGHT:
  case IO_REDIRECTION_LEFT:
    free(io_redirection.file);
    break;

  case IO_REDIRECTION_EMPTY:
    break;
  }
}
