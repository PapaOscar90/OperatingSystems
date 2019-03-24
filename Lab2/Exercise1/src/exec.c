#include "exec.h"

void free_exec(Exec exec) {
  switch (exec.type) {
  case EXEC_ERR:
    break;
  case EXEC_COMMAND:
    break;
  case EXEC_EXIT:
    break;
  }
}
