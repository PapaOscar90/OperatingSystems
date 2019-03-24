#ifndef MACROS_H
#define MACROS_H

#include <stdio.h>
#include <stdlib.h>

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

/// An UNIMPLEMENTED macro that displays a formatted string with varadic
/// arguments, alongside the file, line, and function that the UNIMPLEMENTED
/// macro is called from. The string that is displayed automatically appends a
/// newline.
/// This macro exits the program and should be used to structure and prototype
/// the structure of an application.
#ifndef UNIMPLEMENTED
#define UNIMPLEMENTED(fmt, args...)                                            \
  fprintf(stderr, "UNIMPLEMENTED: %s:%d:%s():" fmt "\n", __FILE__, __LINE__,   \
          __func__, ##args);                                                   \
  exit(EXIT_FAILURE);
#endif

/// An UNREACHABLE macro that displays a formatted string with varadic
/// arguments, alongside the file, line, and function that the UNREACHABLE macro
/// is called from. The string that is displayed automatically appends a
/// newline.
/// This macro exits the program and should be used to represent states that
/// should logically not be reachable.
#ifndef UNREACHABLE
#define UNREACHABLE(fmt, args...)                                              \
  fprintf(stderr, "UNREACHABLE: %s:%d:%s():" fmt "\n", __FILE__, __LINE__,     \
          __func__, ##args);                                                   \
  exit(EXIT_FAILURE);
#endif

#endif /* MACROS_H */
