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

int isSameFile(int file1, int file2) {
  // TODO: Syscalls
  return 1;
}

int main() {
  char directoryPath[PATH_MAX];
  int pathLength = readLine(directoryPath, PATH_MAX, stdin);

  DIR *dirp = opendir(directoryPath);
  if (dirp == NULL) {
    fprintf(stderr, "Unable to open directory at path: %s\n", directoryPath);
    exit(EXIT_FAILURE);
  }
  chdir(directoryPath);

  while (dirp) {
    struct dirent *dp;
    if ((dp = readdir(dirp)) != NULL) {
      // If the file matches "." or ".." skip it
      if (!strcmp(dp->d_name, ".") || !strcmp(dp->d_name, ".."))
        continue;

      struct stat sb;
      if (stat(dp->d_name, &sb) == -1) {
        perror("stat");
        exit(EXIT_FAILURE);
      }

      if (S_ISDIR(sb.st_mode)) {
        printf("Directory: %s\n", dp->d_name);
      } else if (S_ISREG(sb.st_mode)) {
        printf("File: %s\n", dp->d_name);
      }

    } else {
      // No files left to process
      closedir(dirp);
      break;
    }
  }

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

  return EXIT_SUCCESS;
}
