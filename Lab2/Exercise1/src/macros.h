#ifndef MACROS_H
#define MACROS_H

#include <stdio.h>

/// A DBG macro that displays a formatted string with varadic arguments,
/// alongside the file, line, and function that the DBG macro is called from.
/// The string that is displayed automatically appends a newline.
///
/// To use the macro, the user should #define DEBUG to some positive value.
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
