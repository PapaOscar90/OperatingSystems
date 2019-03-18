#ifndef REDIRECTION_H
#define REDIRECTION_H

typedef enum {
  LEFT_RIGHT, // < (filename) > (filename)
  RIGHT_LEFT, // > (filename) < (filename)
  RIGHT,      // > (filename)
  LEFT,       // < (filename)
  NONE,       // empty
} RedirectionType;

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

#endif /* REDIRECTION_H */
