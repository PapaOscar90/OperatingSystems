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

#ifndef LINE_MAX
#define LINE_MAX 4096
#endif

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

int ARRAY_POS = 0;

int isSameFile(char *path1, char *path2) {
  // Variables to hold the information on the files specified by the provided
  // paths.
  struct stat sb1;
  struct stat sb2;
  // Check if there was an error `stat`ing the files.
  if (stat(path1, &sb1) == -1 || stat(path2, &sb2)) {
    perror("stat");
    exit(EXIT_FAILURE);
  }

  // If the files do not have the same size there is no need to compare them
  if (sb1.st_size != sb2.st_size) {
    return 0;
  }

  // As the files have the same size, they are opened so that their contents may
  // be compared.
  // TODO not null checks
  FILE *fp1 = fopen(path1, "r");
  FILE *fp2 = fopen(path2, "r");

  // Iterate through each byte in the files
  for (int i = 0; i < sb1.st_size; i++) {
    char byte1, byte2;
    fread(&byte1, 1, 1, fp1);
    fread(&byte2, 1, 1, fp2);

    if (byte1 != byte2) {
      // If any pairs of bytes are different, we may return early
      fclose(fp1);
      fclose(fp2);
      return 0;
    }
  }

  // No bytes were different and thus the files are identical
  fclose(fp1);
  fclose(fp2);
  return 1;
}

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

size_t getFilesInDirectoryRec(DIR *dirp, char *newPath, char **files) {
  // Store the original path to return
  // TODO there may be some more standard mechanism when using `chdir` then
  // simply storing the original working directory to return to.
  char originalPath[PATH_MAX + 1];
  getcwd(originalPath, PATH_MAX);

  size_t numberOfFiles = 0;

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
        numberOfFiles += getFilesInDirectoryRec(subdirp, dp->d_name, files);

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
char *replaceFirstNChar(char *string, size_t n, char *replacement) {
  size_t replacedLen = strlen(string) - n + strlen(replacement) + 1;
  char *replaced = safeCalloc(replacedLen, sizeof(*replaced));

  for (size_t i = 0; i < strlen(replacement); i++) {
    replaced[i] = replacement[i];
  }
  for (size_t i = strlen(replacement); i < replacedLen - 1; i++) {
    replaced[i] = string[i + n - 1];
  }

  replaced[replacedLen - 1] = '\0';
  return replaced;
}

size_t getFilesInDirectory(char *directoryPath, char **files) {
  DIR *dirp = opendir(directoryPath);
  if (dirp == NULL) {
    fprintf(stderr, "Unable to open directory at path: %s\n", directoryPath);
    exit(EXIT_FAILURE);
  }

  size_t numberOfFiles = getFilesInDirectoryRec(dirp, directoryPath, files);

  char currentPath[PATH_MAX + 1];
  getcwd(currentPath, PATH_MAX);

  for (size_t i = 0; i < numberOfFiles; i++) {
    char *bkp = files[i];
    files[i] = replaceFirstNChar(files[i], strlen(currentPath), ".");
    free(bkp);
  }

  return numberOfFiles;
}

void printDuplicateFiles(char **files, size_t numberOfFiles) {
  /* // Check every element to see if it matches another element in the
     array */
  for (size_t i = 0; i < numberOfFiles; i++) {
    for (size_t j = (i + 1); j < numberOfFiles; j++) {
      if (isSameFile(files[i], files[j])) {
        printf("\"%s\" and \"%s\" are the same file\n", files[i], files[j]);
      }
    }
  }
}

int main() {
  char *directoryPath = readLine(LINE_MAX, stdin);
  char **files = calloc(1000, sizeof(*files));
  chdir(directoryPath);
  size_t numberOfFiles = getFilesInDirectory(".", files);
  printDuplicateFiles(files, numberOfFiles);

  /* Clean up resources*/
  for (size_t i = 0; i < numberOfFiles; i++) {
    free(files[i]);
  }
  free(files);
  free(directoryPath);

  return EXIT_SUCCESS;
}
