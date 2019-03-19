#include "../src/parse.h"
#include "bdd-for-c.h"

spec("skip_whitespace") {
  static const char *test_string = "a bcde     fghij     ";

  it("should be at the next non whitespace character when it exists") {
    size_t position;
    size_t expected;

    position = 0; // 'a'
    expected = 0; // 'a'
    skip_whitespace(test_string, position, &position);
    check(position == expected);

    position = 1; // ' '
    expected = 2; // 'b'
    skip_whitespace(test_string, position, &position);
    check(position == expected);

    position = 6;  // ' '
    expected = 11; // 'f'
    skip_whitespace(test_string, position, &position);
    check(position == expected);
  }

  it("should do nothing if the start value is out of bounds") {
    size_t position = 7;
    size_t original_position = position;
    size_t out_of_bounds = strlen(test_string);

    skip_whitespace(test_string, out_of_bounds, &position);
    check(position == original_position);
  }

  it("should put the caller at the end of the string if the remainder of the "
     "string is solely whitespace") {
    size_t position = 16; // ' '
    size_t len = strlen(test_string);
    skip_whitespace(test_string, position, &position);
    check(position == len);
  }
}
