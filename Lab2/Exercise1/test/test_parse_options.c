#include "../src/parse.h"
#include "bdd_for_c.h"

spec("parse_options") {
  it("should parse a normal set of options properly") {
    const char *test_command = "echo x y z";
    size_t position = 4;

    const char *expected_options = "x y z";
    size_t expected_position = strlen(test_command);

    Parse actual = parse_options(test_command, position, &position);
    check(actual.type == PARSE_OPTIONS);
    check(strcmp(actual.options, expected_options) == 0);
    check(position == expected_position);
  }

  it("should leading skip whitespace") {
    const char *test_command = "echo         ls";
    size_t position = 4;

    const char *expected_command = "ls";
    size_t expected_position = strlen(test_command);

    Parse actual = parse_options(test_command, position, &position);
    check(actual.type == PARSE_OPTIONS);
    check(strcmp(actual.options, expected_command) == 0);
    check(position == expected_position);
  }

  it("handle quotes and strip them correctly") {
    const char *test_command = "echo '   echo   ls'";
    size_t position = 4;

    const char *expected_options = "   echo   ls";
    size_t expected_position = strlen(test_command);

    Parse actual = parse_options(test_command, position, &position);
    check(actual.type == PARSE_OPTIONS);
    check(strcmp(actual.options, expected_options) == 0);
    check(position == expected_position);
  }

  it("should be null if there are no options") {
    const char *test_command = "echo     ";
    size_t position = 4;

    size_t expected_position = strlen(test_command);
    Parse actual = parse_options(test_command, position, &position);
    check(actual.type == PARSE_OPTIONS);
    check(actual.options == NULL);
    check(position == expected_position);
  }

  it("should handle quoted whitespace correctly") {
    const char *test_command = "echo '                 '";
    size_t position = 4;
    size_t expected_position = strlen(test_command);
    const char *expected_options = "                 ";
    Parse actual = parse_options(test_command, position, &position);
    check(actual.type == PARSE_OPTIONS);
    check(strcmp(actual.options, expected_options) == 0);
    check(position = expected_position);
  }

  it("should not parse special characters like '&' in the command name") {
    const char *test_command = "echo lsasa&";
    size_t position = 4;
    size_t expected_position = strlen(test_command) - 1;
    const char *expected_options = "lsasa";
    Parse actual = parse_options(test_command, position, &position);
    check(actual.type == PARSE_OPTIONS);
    check(strcmp(actual.options, expected_options) == 0);
    check(position = expected_position);
  }

  it("should be null if the arguments consist only of unquoted whitespace") {
    const char *test_command = "ls &     ";
    size_t position = 2;
    size_t expected_position = 2;

    Parse actual = parse_options(test_command, position, &position);
    check(actual.type == PARSE_OPTIONS);
    check(actual.options == NULL);
    check(position = expected_position);
  }

  it("should not capture whitespace after the arguments") {
    const char *test_command = "ls x y z    ";
    size_t position = 2;
    size_t expected_position = strlen(test_command);
    const char *expected_options = "x y z";

    Parse actual = parse_options(test_command, position, &position);
    check(actual.type == PARSE_OPTIONS);
    check(strcmp(actual.options, expected_options) == 0);
    check(position = expected_position);
  }

  it("should not capture whitespace after the arguments when followed by an "
     "'&'") {
    const char *test_command = "ls x y z    &";
    size_t position = 2;
    size_t expected_position = strlen(test_command) - 1;
    const char *expected_options = "x y z";

    Parse actual = parse_options(test_command, position, &position);
    check(actual.type == PARSE_OPTIONS);
    check(strcmp(actual.options, expected_options) == 0);
    check(position = expected_position);
  }
}
