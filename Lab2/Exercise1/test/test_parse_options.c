#include "../src/parse.h"
#include "bdd-for-c.h"

spec("parse_options") {
  it("should parse a normal set of options properly") {
    const char *test_command = "echo x y z";
    size_t position = 4;

    const char *expected_options = "x y z";
    size_t expected_position = strlen(test_command);

    ParseResult actual = parse_options(test_command, position, &position);
    check(actual.type == OPTIONS);
    check(strcmp(actual.options, expected_options) == 0);
    check(position == expected_position);
  }

  it("should leading skip whitespace") {
    const char *test_command = "echo         ls";
    size_t position = 4;

    const char *expected_command = "ls";
    size_t expected_position = strlen(test_command);

    ParseResult actual = parse_options(test_command, position, &position);
    check(actual.type == OPTIONS);
    check(strcmp(actual.options, expected_command) == 0);
    check(position == expected_position);
  }

  it("handle quotes and strip them correctly") {
    const char *test_command = "echo '   echo   ls'";
    size_t position = 4;

    const char *expected_options = "   echo   ls";
    size_t expected_position = strlen(test_command);

    ParseResult actual = parse_options(test_command, position, &position);
    check(actual.type == OPTIONS);
    check(strcmp(actual.options, expected_options) == 0);
    check(position == expected_position);
  }

  it("should be null if there are no options") {
    const char *test_command = "echo     ";
    size_t position = 4;

    size_t expected_position = strlen(test_command);
    ParseResult actual = parse_options(test_command, position, &position);
    check(actual.type == OPTIONS);
    check(actual.options == NULL);
    check(position == expected_position);
  }

  it("should handle quoted whitespace correctly") {
    const char *test_command = "echo '                 '";
    size_t position = 4;
    size_t expected_position = strlen(test_command);
    const char *expected_options = "                 ";
    ParseResult actual = parse_options(test_command, position, &position);
    check(actual.type == OPTIONS);
    check(strcmp(actual.options, expected_options) == 0);
    check(position = expected_position);
  }

  it("should not parse special characters like '&' in the command name") {
    const char *test_command = "echo lsasa&";
    size_t position = 4;
    size_t expected_position = strlen(test_command) - 1;
    const char *expected_options = "lsasa";
    ParseResult actual = parse_options(test_command, position, &position);
    check(actual.type == OPTIONS);
    check(strcmp(actual.options, expected_options) == 0);
    check(position = expected_position);
  }
}
