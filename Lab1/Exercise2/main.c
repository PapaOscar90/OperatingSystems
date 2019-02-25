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

#define END_NUMBER 50
#define MSG_LEN 256

void checkedPipe(int fd[2]) {
  if (pipe(fd) == -1) {
    fprintf(stderr, "Could not make pipe\n");
    exit(EXIT_FAILURE);
  }
}

pid_t checkedFork(void) {
  pid_t id = fork();
  if (id == -1) {
    fprintf(stderr, "Could not fork process\n");
    exit(EXIT_FAILURE);
  }

  return id;
}

int spawnChildren(int numberOfChildren, int relativeId, int pipeEven[2],
                  int pipeOdd[2]) {
  for (int i = 0; i < numberOfChildren; i++) {
    checkedPipe(relativeId % 2 == 0 ? pipeEven : pipeOdd);
    if (checkedFork() != 0) {
      // Parent
      return relativeId;
    } else {
      relativeId++;
    }
  }

  return relativeId;
}

int main() {
  // Disable buffering of stdout
  setbuf(stdout, NULL);

  // Incremented by the threads until END_NUMBER
  int count = 0;
  int numberOfProcesses;
  scanf("%d", &numberOfProcesses);
  int relativeId = 0;

  if (numberOfProcesses == 1) {
    for (int i = 0; i < END_NUMBER + 1; i++) {
      printf("relative pid=%d: %d\n", relativeId, count);
      count++;
    }
    return EXIT_SUCCESS;
  }

  char msg[MSG_LEN] = "";
  int pipeWrap[2];
  int pipeEven[2];
  int pipeOdd[2];

  checkedPipe(pipeWrap);
  relativeId =
      spawnChildren(numberOfProcesses - 1, relativeId, pipeEven, pipeOdd);

  // Jump start process
  if (relativeId == 0) {
    printf("relative pid=%d: %d\n", relativeId, count);
    count++;
    sprintf(msg, "%d", count);
    write(pipeEven[1], msg, MSG_LEN);
  }

  while (1) {
    if (relativeId == 0) {
      read(pipeWrap[0], msg, MSG_LEN);
      count = atoi(msg);
      if (count == -1 || count > END_NUMBER) {
        sprintf(msg, "%d", -1);
        write(pipeEven[1], msg, MSG_LEN);
        break;
      }
      printf("relative pid=%d: %d\n", relativeId, count);
      count++;
      sprintf(msg, "%d", count);
      write(pipeEven[1], msg, MSG_LEN);
    } else if (relativeId == numberOfProcesses - 1) {
      read((relativeId % 2 == 0 ? pipeOdd[0] : pipeEven[0]), msg, MSG_LEN);
      count = atoi(msg);
      if (count == -1 || count > END_NUMBER) {
        sprintf(msg, "%d", -1);
        write(pipeWrap[1], msg, MSG_LEN);
        break;
      }
      printf("relative pid=%d: %d\n", relativeId, count);
      count++;
      sprintf(msg, "%d", count);
      write(pipeWrap[1], msg, MSG_LEN);
    } else {
      read(relativeId % 2 == 0 ? pipeOdd[0] : pipeEven[0], msg, MSG_LEN);
      count = atoi(msg);
      if (count == -1 || count > END_NUMBER) {
        sprintf(msg, "%d", -1);
        write(relativeId % 2 == 0 ? pipeEven[1] : pipeOdd[1], msg, MSG_LEN);
        break;
      }
      printf("relative pid=%d: %d\n", relativeId, count);
      count++;
      sprintf(msg, "%d", count);
      write(relativeId % 2 == 0 ? pipeEven[1] : pipeOdd[1], msg, MSG_LEN);
    }
  }

  for (int i = 0; i < numberOfProcesses - 1; i++) {
    waitpid(-1, NULL, 0);
  }

  return EXIT_SUCCESS;
}
