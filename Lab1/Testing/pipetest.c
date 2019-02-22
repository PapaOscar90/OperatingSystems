#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

/*
 * Pipe Testing
 * A pipe is a unidirectional interprocess communication tool
 * It has a read end, and write end.
 *      0 = read
 *      1 = write
 */
int main(int argc, char *argv[]) {
  int fd[2], status;
  char msg[256];

  if (pipe(fd) < 0) {
    fprintf(stderr, "Could not make pipe\n");
    return EXIT_FAILURE;
  }

  if (fork() != 0) {
    /* Parent runs this */
    close(fd[0]); // fd[0] is used for reading

    sprintf(msg, "Hello World!\n");

    // fd[1] is used for writing
    write(fd[1], msg, 256);

    // Done writing
    close(fd[1]);
  } else {
    /* Child runs this */
    close(fd[1]);

    // Read from the READ side 0
    read(fd[0], msg, 256);

    printf(msg);

    close(fd[0]);
  }
  waitpid(-1, &status, 0);

  return EXIT_SUCCESS;
}
