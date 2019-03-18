#ifndef MACROS_H
#define MACROS_H

#include <stdio.h>

#ifndef DBG
#if defined(DEBUG) && DEBUG > 0
#define DBG(fmt, args...)                                                      \
  fprintf(stderr, "DEBUG: %s:%d:%s():" fmt "\n", __FILE__, __LINE__, __func__, \
          ##args);
#else
#define DBG(fmt, args...)
#endif
#endif

#endif /* MACROS_H */
