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
#include <unistd.h>

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

#ifndef FILENAME_MAX
#define FILENAME_MAX 256
#endif

int isSameFile(int file1, int file2) {
  // TODO: Syscalls
  return 1;
}

int main() {
  char directoryPath[PATH_MAX];
  int pathLength = readLine(directoryPath, PATH_MAX, stdin);

  printf("%d\n", pathLength);
  printf("%s\n", directoryPath);

  /* int numberOfFiles = 5; */
  /* int files[5] = {0, 0, 0, 0, 0}; */

  /* // Check every element to see if it matches another element in the array */
  /* for (int i = 0; i < numberOfFiles; i++) { */
  /*   for (int j = (i + 1); j < numberOfFiles; j++) { */
  /*     if (isSameFile(files[i], files[j])) { */
  /*       printf("%d and %d are the same file\n", i, j); */
  /*     } */
  /*   } */
  /* } */

  return 0;
}
