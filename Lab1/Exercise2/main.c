/*
 * Ring of Communication
 *
 * Troy Karan Harrison - s2841886
 * Philip Oetinger     - s2966018
 *
 * This program reads an integer for the number of processes to open.
 * It will then count to 50, in a ring, one process after the next. Each
 * process will talk with the process to it's "right". 0->1->2->3->0
 *
 * Each parent<->child combo will have 2 pipes, for bidirectional communication.
 * Using pipe a will go deeper into the tree and print the next number, while
 * using pipe b will go back up to the root, passing the number along
 */

#include "util.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  int origParentID, status;
  int child;
  int endNumber = 50;
  int number = 0;

  char msg[256];

  // Store the original processID for the final child to communicate with
  origParentID = getpid();
  int pipeWrap[2];
  if (pipe(pipeWrap) < 0) {
    fprintf(stderr, "Couldn not make pipe.");
  }
  int pipeEven[2];
  int pipeOdd[2];

  int numberOfChildrenToMake = 5;
  int numberOfChildren = 5;
  int myRelativeID = 0;

  printf("Hello, I am the super_parent. My ID is %d, and RID is %d\n", getpid(),
         myRelativeID);

  while (numberOfChildrenToMake > 0) {
    if ((myRelativeID % 2 == 0 && pipe(pipeEven) < 0) ||
        (myRelativeID % 2 == 1 && pipe(pipeOdd) < 0)) {
      fprintf(stderr, "Could not make pipe\n");
      return EXIT_FAILURE;
    }
    child = fork();
    if (child != 0) {
      // Parent runs this
      printf("ID= %d, number=%d, created=%d\n", myRelativeID, number, child);
      if (myRelativeID == 0) {
        printf("relative pid=%d: %d\n", myRelativeID, number);
        number++;
        sprintf(msg, "%d", number);
        write(pipeEven[1], msg, 256);
        close(pipeEven[0]);
      }
      break;
    } else {
      myRelativeID++;
      numberOfChildrenToMake--;
      printf("Hello World, I am RID, %d (%d).\n", getpid(), myRelativeID);
    }
  }

  while (number < endNumber) {
    if (myRelativeID == 0) {
      printf("Relative ID: %d, trying to read from pipeWrap.\n", myRelativeID);
      read(pipeWrap[0], msg, 256);
      number = atoi(msg);
      printf("relative pid=%d: %d", myRelativeID, number);
      number++;
      sprintf(msg, "%d", number);
      write(pipeEven[1], msg, 256);
    } else if (myRelativeID == numberOfChildren) {
      printf("Relative ID: %d, trying to read from pipe?.\n", myRelativeID);
      read((myRelativeID % 2 == 0 ? pipeEven[0] : pipeOdd[0]), msg, 256);
      number = atoi(msg);
      printf("relative pid=%d: %d", myRelativeID, number);
      number++;
      sprintf(msg, "%d", number);
      write(pipeWrap[1], msg, 256);
    } else if (myRelativeID % 2 == 1) {
      printf("Relative ID: %d, trying to read from pipeEven.\n", myRelativeID);
      read(pipeEven[0], msg, 256);
      number = atoi(msg);
      printf("relative pid=%d: %d", myRelativeID, number);
      number++;
      sprintf(msg, "%d", number);
      write(pipeOdd[1], msg, 256);
    } else {
      printf("Relative ID: %d, trying to read from pipeOdd.\n", myRelativeID);
      read(pipeOdd[0], msg, 256);
      number = atoi(msg);
      printf("relative pid=%d: %d", myRelativeID, number);
      number++;
      sprintf(msg, "%d", number);
      write(pipeEven[1], msg, 256);
    }
  }

  for (int i = 0; i < 5; i++) {
    waitpid(-1, &status, 0);
  }
  return EXIT_SUCCESS;
}
