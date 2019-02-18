/*
 * Parsing Command Line Arguments
 *
 * Troy Karan Harrison - s2841886
 * Philip Oetinger     - s2966018
 *
 * This program takes in a command (optional quotes) with arguments
 * and runs a new process that executes the command with arguments
 *
 */

#include "util.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

#ifndef FILENAME_MAX
#define FILENAME_MAX 256
#endif

// This function will find and seperate the command from the rest of the arguments
void seperateCommandFromArguments(char* input, char* command, char* arguments){
  // If the command starts with ", look for the closing and make that the command
  if (input[0] == '"') {
    int secondQuote = 1;

    while (input[secondQuote] != '"') {
      secondQuote++;
    }

    // Now copy the command and arguments to their respective strings
    strncpy(command, input + 1, secondQuote - 1);
    strncpy(arguments, input + secondQuote + 2, PATH_MAX);
  } else {
    // If no quotes used, find the next ' ' signifying end of command
    int endOfCommand = 1;

    while (input[endOfCommand] != ' ') {
      endOfCommand++;
    }

    // Now copy the command and arguments to their respective strings
    strncpy(command, input, endOfCommand);
    strncpy(arguments, input + endOfCommand + 1, PATH_MAX);
  }
}

// Go through all the paths from getenv to check if cmd exists
void findFilePath(char* allPaths, char* command, char* fullFilePath){
  struct stat buffer;
  int exists = 1;

  char *token = strtok(allPaths, ":");
  // Go through all the paths, and if a command is found, break with fullFilePath containing path
  while (token != NULL ){
    sprintf(fullFilePath, "%s/%s", token, command);
    exists = stat(fullFilePath, &buffer);
    if (exists == 0 && S_ISREG(buffer.st_mode)){
      break;
    }

    token = strtok(NULL,":");
  }
}


int main() {
  char line[PATH_MAX];
  char execCmd[FILENAME_MAX];
  char arguments[PATH_MAX];
  int child, status;

  readLine(line, PATH_MAX, stdin);

  seperateCommandFromArguments(line, execCmd, arguments);

  char *allPaths = getenv("PATH");
  char fullFilePath[PATH_MAX];
  findFilePath(allPaths, execCmd, fullFilePath);

  // Create a child process that will execute the command
  child = fork();
  if (child < 0) {
    fprintf(stderr, "Fork failed: aborted\n");
    return EXIT_FAILURE;
  }


  if (child != 0) {
    /* Parent runs nothing */
  } else {
    /* Child runs the new core with arguments that were passed in */
    char *newargv[3];
    newargv[0] = execCmd;
    newargv[1] = arguments;
    newargv[2] = NULL;

    execve(fullFilePath, newargv, NULL);
    printf("Command %s not found!\n", execCmd);
    exit(EXIT_FAILURE);
  }
  waitpid(-1, &status, 0);

  return EXIT_SUCCESS;
}
