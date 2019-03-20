#include "../src/parse.h"
#include "bdd_for_c.h"

spec("parse_command_name") {
  it("should parse a normal command properly") {
    const char *test_command = "echo";
    size_t position = 0;

    const char *expected_command = "echo";
    size_t expected_position = 4;

    Parse actual = parse_command_name(test_command, position, &position);
    check(actual.type == PARSE_COMMAND_NAME);
    check(strcmp(actual.command_name, expected_command) == 0);
    check(position == expected_position);
  }

  it("should leading skip whitespace and stop at whitespace") {
    const char *test_command = "   echo   ls";
    size_t position = 0;

    const char *expected_command = "echo";
    size_t expected_position = 7;

    Parse actual = parse_command_name(test_command, position, &position);
    check(actual.type == PARSE_COMMAND_NAME);
    check(strcmp(actual.command_name, expected_command) == 0);
    check(position == expected_position);
  }

  it("handle quotes and strip them correctly") {
    const char *test_command = "'   echo   ls'";
    size_t position = 0;

    const char *expected_command = "'   echo   ls'";
    size_t expected_position = 14;

    Parse actual = parse_command_name(test_command, position, &position);
    check(actual.type == PARSE_COMMAND_NAME);
    check(strcmp(actual.command_name, expected_command) == 0);
    check(position == expected_position);
  }

  it("should be error if there is no command") {
    const char *test_command = "               ";
    size_t position = 0;
    Parse actual = parse_command_name(test_command, position, &position);
    check(actual.type == PARSE_ERR);
  }

  it("should handle quoted whitespace correctly") {
    const char *test_command = "'                 '";
    size_t position = 0;
    size_t expected_position = strlen(test_command);
    const char *expected_command = "'                 '";
    Parse actual = parse_command_name(test_command, position, &position);
    check(actual.type == PARSE_COMMAND_NAME);
    check(strcmp(actual.command_name, expected_command) == 0);
    check(position = expected_position);
  }

  it("should not parse special characters like '&' in the command name") {
    const char *test_command = "echo&";
    size_t position = 0;
    size_t expected_position = strlen(test_command) - 1;
    const char *expected_command = "echo";
    Parse actual = parse_command_name(test_command, position, &position);
    check(actual.type == PARSE_COMMAND_NAME);
    check(strcmp(actual.command_name, expected_command) == 0);
    check(position = expected_position);
  }

  it("should leave interior quotes in the command") {
    const char *test_command = "'echo'";
    size_t position = 0;
    size_t expected_position = strlen(test_command);
    const char *expected_command = "'echo'";
    Parse actual = parse_command_name(test_command, position, &position);
    check(actual.type == PARSE_COMMAND_NAME);
    check(strcmp(actual.command_name, expected_command) == 0);
    check(position = expected_position);
  }
}
