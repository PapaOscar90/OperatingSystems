#include "../src/util.h"
#include "bdd-for-c.h"

spec("unquote_string") {
  it("should leave an unquoted string unchanged") {
    const char *input = "this is a test";
    const char *expected = input;

    char *output = unquote_string(input);

    check(strcmp(output, expected) == 0);
    free(output);
  }

  it("should be empty if a string consists solely of matched quotes") {
    const char *input = "\"\"''\"\"''''\"\"";
    const char *expected = "";

    char *output = unquote_string(input);

    check(strcmp(output, expected) == 0);
    free(output);
  }

  it("should correctly strip out a single set of quotes (\")") {
    const char *input = "\"this is a test\"";
    const char *expected = "this is a test";

    char *output = unquote_string(input);

    check(strcmp(output, expected) == 0);
    check(strlen(output) == strlen(expected));
    free(output);
  }

  it("should correctly strip out a single set of quotes (\')") {
    const char *input = "'this is a test'";
    const char *expected = "this is a test";

    char *output = unquote_string(input);

    check(strcmp(output, expected) == 0);
    check(strlen(output) == strlen(expected));
    free(output);
  }

  it("should leave opposing quotes within the string") {
    const char *input1 = "\"'this' 'is' 'a' 'test'\"";
    const char *expected1 = "'this' 'is' 'a' 'test'";

    const char *input2 = "'\"this\" \"is\" \"a\" \"test\"'";
    const char *expected2 = "\"this\" \"is\" \"a\" \"test\"";

    char *output1 = unquote_string(input1);
    char *output2 = unquote_string(input2);

    check(strcmp(output1, expected1) == 0);
    check(strlen(output1) == strlen(expected1));
    check(strcmp(output2, expected2) == 0);
    check(strlen(output2) == strlen(expected2));

    free(output1);
    free(output2);
  }

  it("should return null if there are unmatched outer quotes") {
    const char *input1 = "\"'this' 'is' 'a' 'test'\"\"";

    const char *input2 = "'\"this\" \"is\" \"a\" \"test\"''";

    char *output1 = unquote_string(input1);
    char *output2 = unquote_string(input2);

    check(output1 == NULL);
    check(output2 == NULL);

    free(output1);
    free(output2);
  }
}
