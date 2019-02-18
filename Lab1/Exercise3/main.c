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

int STUCK_COUNTER = 0;

int isSameFile(char *path1, char *path2) {
  struct stat sb1;
  struct stat sb2;
  if (stat(path1, &sb1) == -1 || stat(path2, &sb2)) {
    perror("stat");
    exit(EXIT_FAILURE);
  }

  // If the files do not have the same size
  if (sb1.st_size != sb2.st_size) {
    return 0;
  }
  // TODO not null checks
  FILE *fp1 = fopen(path1, "r");
  FILE *fp2 = fopen(path2, "r");

  for (int i = 0; i < sb1.st_size; i++) {

    char byte1, byte2;
    fread(&byte1, 1, 1, fp1);
    fread(&byte2, 1, 1, fp2);
    if (byte1 != byte2) {
      fclose(fp1);
      fclose(fp2);
      return 0;
    }
  }

  fclose(fp1);
  fclose(fp2);
  return 1;
}

int ARRAY_POS = 0;

char *realPath(const char *filename) {
  char *absolutePath = calloc(PATH_MAX + 1, sizeof(*absolutePath));
  getcwd(absolutePath, PATH_MAX);
  size_t pathOffset = strlen(absolutePath);
  if (pathOffset > 1) {
    absolutePath[pathOffset] = '/';
    pathOffset++;
  }
  for (size_t i = 0; i < strlen(filename) + 1; i++) {
    absolutePath[pathOffset + i] = filename[i];
  }
  return absolutePath;
}

int getFilesInDirectory(DIR *dirp, char *newPath, char **files) {
  // Store the original path to return
  // TODO there may be some more standard mechanism when using `chdir` then
  // simply storing the original working directory to return to.
  char originalPath[PATH_MAX];
  getcwd(originalPath, PATH_MAX);

  int numberOfFiles = 0;

  // Switch the working directory so the relative file names provided by
  // `readdir` are relevant.
  chdir(newPath);
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
        // The file is a directory
        DIR *subdirp = opendir(dp->d_name);
        if (subdirp == NULL) {
          fprintf(stderr, "Unable to open directory at path: %s\n", dp->d_name);
          exit(EXIT_FAILURE);
        }
        numberOfFiles += getFilesInDirectory(subdirp, dp->d_name, files);

      } else if (S_ISREG(sb.st_mode)) {
        // The file is a regular file
        numberOfFiles++;
        char *actualpath = realPath(dp->d_name);
        files[ARRAY_POS] = actualpath;
        ARRAY_POS++;
      }

    } else {
      // No files left to process
      closedir(dirp);
      break;
    }
  }

  chdir(originalPath);
  return numberOfFiles;
}

int main() {
  char directoryPath[PATH_MAX];
  readLine(directoryPath, PATH_MAX, stdin);

  DIR *dirp = opendir(directoryPath);
  if (dirp == NULL) {
    fprintf(stderr, "Unable to open directory at path: %s\n", directoryPath);
    exit(EXIT_FAILURE);
  }

  char **files = calloc(1000, sizeof(*files));

  int numberOfFiles = getFilesInDirectory(dirp, directoryPath, files);

  /* // Check every element to see if it matches another element in the
     array */
  for (int i = 0; i < numberOfFiles; i++) {
    for (int j = (i + 1); j < numberOfFiles; j++) {
      if (isSameFile(files[i], files[j])) {
        printf("%s and %s are the same file\n", files[i], files[j]);
      }
    }
  }

  for (int i = 0; i < numberOfFiles; i++) {
    free(files[i]);
  }
  free(files);

  return EXIT_SUCCESS;
}
