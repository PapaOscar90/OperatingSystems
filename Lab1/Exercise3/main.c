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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int isSameFile(int file1, int file2);

int main() {
  char *lineIn;
  size_t lineLen;

  int numberOfFiles = 5;
  int files[5] = {0, 0, 0, 0, 0};

  // Check every element to see if it matches another element in the array
  for (int i = 0; i < numberOfFiles; i++) {
    for (int j = (i + 1); j < numberOfFiles; j++) {
      if (isSameFile(files[i], files[j])) {
        printf("%d and %d are the same file\n", i, j);
      }
    }
  }

  return 0;
}

int isSameFile(int file1, int file2) {
  // TODO: Syscalls
  return 1;
}
