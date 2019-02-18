/*
 * File System Traversal
 *
 * Troy Karan Harrison - s2841886
 * Philip Oetinger     - s2966018
 *
 * Traverses the file system starting from a directory specified over stdin. It
 * compares the files contained within the directory for equality and prints all
 * unique pairs that have the same content.
 *
 */

#include "util.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

#ifndef FILENAME_MAX
#define FILENAME_MAX 256
#endif



int main(){
  char cmdLine[PATH_MAX];
  char execCmd[FILENAME_MAX];
  char argumentList[PATH_MAX];
  int parent, child, status;

  readLine(cmdLine, PATH_MAX, stdin);


  printf("%s\nStarts with: %c\n", cmdLine, cmdLine[0]);
  if(cmdLine[0] == '"'){
    int secondQuote=1;

    while(cmdLine[secondQuote] != '"'){
      secondQuote++;
    }
    strncpy(execCmd,cmdLine+1,secondQuote-1);
    strncpy(argumentList,cmdLine+secondQuote+2, PATH_MAX);
  } else {
    int endOfCommand = 1;

    while(cmdLine[endOfCommand] != ' '){
      endOfCommand++;
    }
    strncpy(execCmd, cmdLine, endOfCommand);
    strncpy(argumentList, cmdLine+endOfCommand+1, PATH_MAX);
  }

  printf("Command: %s\n", execCmd);
  printf("Arguments: %s\n", argumentList);
  char *fullPath = getenv("PATH");
  printf("PATH: %s\n", fullPath);
  strcat(fullPath, "/");
  strcat(fullPath,execCmd);
  printf("CommandPath: %s\n", fullPath);

  parent = getpid();

  //child = fork();
  if (child < 0){
    fprintf(stderr, "Fork failed: aborted\n");
    return EXIT_FAILURE;
  }

  if (child != 0){
    /* Parent runs this */
  } else {
    char *newargv[3];
    newargv[0] = execCmd;
    newargv[1] = argumentList;
    newargv[3] = NULL;

    printf("Child staring new core...\n");
    execve(execCmd, newargv, NULL);
    perror(execve);
    exit(EXIT_FAILURE);
  }
  printf("Parent waiting...\n");
  waitpid(-1, &status, 0);
  printf("Child is complete, ending...\n");

  return EXIT_SUCCESS;
}
