#ifndef EXEC_H
#define EXEC_H

typedef enum {
  EXEC_ERR,
  EXEC_COMMAND,
  EXEC_EXIT,
} ExecType;

typedef struct {
  ExecType type;
} Exec;

void free_exec(Exec exec);

#endif /* EXEC_H */
