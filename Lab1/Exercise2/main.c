/*
 * Ring of Communication
 *
 * Troy Karan Harrison - s2841886
 * Philip Oetinger     - s2966018
 *
 * This program reads an integer for the number of processes to open.
 * It will then count to 50, in a ring, one process after the next. Each
 * process will talk with the process to it's "right". For 4 processes the ring
 * of communication is:
 *   0->1->2->3->0
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define END_NUMBER 50
#define MSG_LEN 256

// pipe which exits on error
void checkedPipe(int fd[2]) {
  if (pipe(fd) == -1) {
    fprintf(stderr, "Could not make pipe\n");
    exit(EXIT_FAILURE);
  }
}

// fork which exits on error
pid_t checkedFork(void) {
  pid_t id = fork();
  if (id == -1) {
    fprintf(stderr, "Could not fork process\n");
    exit(EXIT_FAILURE);
  }

  return id;
}

// Spawns the children, sets their relative id, and creates their pipes
int spawnChildren(int numberOfChildren, int relativeId, int pipeEven[2],
                  int pipeOdd[2]) {
  // For each child
  for (int i = 0; i < numberOfChildren; i++) {
    // Setup its pipe
    checkedPipe(relativeId % 2 == 0 ? pipeEven : pipeOdd);
    if (checkedFork() != 0) {
      // Return its id
      return relativeId;
    } else {
      // Set the child to the next id
      relativeId++;
    }
  }

  return relativeId;
}

void processLoop(size_t numberOfProcesses) {

  // Incremented by the processes until END_NUMBER
  int count = 0;

  // The relative id of each process with respect to the root process
  size_t relativeId = 0;

  // The message buffer to read messages to and write message from
  char msg[MSG_LEN] = "";

  // The pipe to handle communication between the last and root process
  int pipeWrap[2];
  checkedPipe(pipeWrap);

  // The pipe to handle
  //   writing by processes with even relative ids and
  //   reading by processes with odd relative ids
  int pipeEven[2];

  // The pipe to handle
  //   writing by processes with odd relative ids and
  //   reading by processes with even relative ids
  int pipeOdd[2];

  // Spawn the children and retrieve the relative id for the current process
  relativeId =
      spawnChildren(numberOfProcesses - 1, relativeId, pipeEven, pipeOdd);

  if (relativeId == 0) {
    // The root jump starts the process
    printf("relative pid=%lu: %d\n", relativeId, count);
    count++;

    // Prepare the message for the next along in the chain
    sprintf(msg, "%d", count);

    // Send the message to the next in the chain.
    write(pipeEven[1], msg, MSG_LEN);
  }

  while (1) {
    int readPipe, writePipe;
    // Set the read and write pipes

    if (relativeId == 0) {
      // The root process reads from the pipe wrapped from the last process in
      // the chain
      readPipe = pipeWrap[0];
      // The root process (relativeId == 0) writes to the even pipe
      writePipe = pipeEven[1];
    } else if (relativeId == numberOfProcesses - 1) {
      // The last process in the chain reads from the pipe according to its id
      readPipe = (relativeId % 2 == 0 ? pipeOdd : pipeEven)[0];
      // The last process in the chain writes to the wrap-around pipe
      writePipe = pipeWrap[1];
    } else {
      // An intermediary process in the chain reads from the pipe according to
      // its id
      readPipe = (relativeId % 2 == 0 ? pipeOdd : pipeEven)[0];
      // An intermediary process in the chain writes to the pipe according to
      // its id
      writePipe = (relativeId % 2 == 0 ? pipeEven : pipeOdd)[1];
    }

    // Read and convert the message
    read(readPipe, msg, MSG_LEN);
    count = atoi(msg);

    if (count == -1 || count > END_NUMBER) {
      // Termination should be communicated through the ring
      sprintf(msg, "%d", -1);
      write(writePipe, msg, MSG_LEN);
      break;
    }

    printf("relative pid=%lu: %d\n", relativeId, count);

    // Update the values and prepare the outgoing message
    count++;
    sprintf(msg, "%d", count);

    // Transmit the message to the next in the chain
    write(writePipe, msg, MSG_LEN);
  }

  // Wait for the processes to be finished to prevent zombification
  for (size_t i = 0; i < numberOfProcesses - 1; i++) {
    waitpid(-1, NULL, 0);
  }
}

int main() {
  // Disable buffering of stdout
  setbuf(stdout, NULL);

  // The number of processes that will be used in the chain
  size_t numberOfProcesses;
  scanf("%lu", &numberOfProcesses);

  if (numberOfProcesses == 1) {
    // For only 1 process process spawning is not needed
    for (int i = 0; i < END_NUMBER + 1; i++) {
      printf("relative pid=%d: %d\n", 0, i);
    }
  } else {
    // Execute the logic using many processes
    processLoop(numberOfProcesses);
  }

  return EXIT_SUCCESS;
}
