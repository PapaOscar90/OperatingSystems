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
  readLine(cmdLine, PATH_MAX, stdin);


  printf("%s\nStarts with: %c\n", cmdLine, cmdLine[0]);
  if(cmdLine[0] == '"'){
    int secondQuote=1;
    while(cmdLine[secondQuote] != '"'){
      secondQuote++;
    }
    strncpy(execCmd,cmdLine+1,secondQuote-1);
  }

  printf("Command: %s\n", execCmd);
}
