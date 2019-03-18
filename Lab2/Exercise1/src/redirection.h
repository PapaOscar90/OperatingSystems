#ifndef REDIRECTION_H
#define REDIRECTION_H

/// Possible types of redirection.
typedef enum {
  LEFT_RIGHT, // < (filename) > (filename)
  RIGHT_LEFT, // > (filename) < (filename)
  RIGHT,      // > (filename)
  LEFT,       // < (filename)
  NONE,       // empty
} RedirectionType;

/// A redirection knows of its type of redirection and based on this knows about
/// 0, 1, or 2 files.
typedef struct {
  RedirectionType type;
  union {
    struct { // LEFT_RIGHT | RIGHT_LEFT
      char *left_file;
      char *right_file;
    };
    struct { // LEFT | RIGHT
      char *file;
    };
    struct { // NONE
    };
  };
} Redirection;

void free_redirection(Redirection redirection);

Redirection create_redirection(RedirectionType type, char *file1, char *file2);

#endif /* REDIRECTION_H */
