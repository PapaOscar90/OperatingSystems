#ifndef IO_REDIRECTION_H
#define IO_REDIRECTION_H

typedef enum {
  IO_REDIRECTION_LEFT_RIGHT,
  IO_REDIRECTION_RIGHT_LEFT,
  IO_REDIRECTION_RIGHT,
  IO_REDIRECTION_LEFT,
  IO_REDIRECTION_EMPTY,
} IoRedirectionType;

typedef struct {
  IoRedirectionType type;
  union {
    struct { // IO_REDIRECTION_{LEFT_RIGHT, RIGHT_LEFT}
      char *file1;
      char *file2;
    };
    struct { // IO_REDIRECTION_{LEFT, RIGHT}
      char *file;
    };
    struct { // IO_REDIRECTION_EMPTY
    };
  };
} IoRedirection;

void free_io_redirection(IoRedirection io_redirection);

#endif /* IO_REDIRECTION_H */
