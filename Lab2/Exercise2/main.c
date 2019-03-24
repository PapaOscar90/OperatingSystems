#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

// fork which exits on error
pid_t checked_fork(void) {
  pid_t id = fork();
  if (id == -1) {
    perror("fork");
    exit(EXIT_FAILURE);
  }
  return id;
}

unsigned int *create_buffer(void) {
  // Values for page set according to:
  // http://man7.org/linux/man-pages/man2/mmap.2.html
  unsigned int *buffer =
      mmap(NULL, sizeof(unsigned int), PROT_READ | PROT_WRITE,
           MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  return buffer;
}

sem_t *create_semaphor(char const *name) {
  // Semaphor name semantics set according to:
  // http://man7.org/linux/man-pages/man7/sem_overview.7.html
  if (name[0] != '/') {
    fprintf(stderr, "Semaphor name: \"%s\" does not begin with a '/'\n", name);
    exit(EXIT_FAILURE);
  }
  for (size_t i = 1; i < strlen(name); ++i) {
    if (name[i] == '/') {
      fprintf(stderr,
              "Semaphor name: \"%s\" must not contain a '/'"
              "after the first character\n",
              name);
      exit(EXIT_FAILURE);
    }
  }
  // Read and write user permission;
  mode_t permissions = S_IRUSR | S_IWUSR;
  sem_t *semaphor = sem_open(name, O_CREAT, permissions, 0);
  return semaphor;
}

static const unsigned int sentinel = 0;

void execute_pipeline(unsigned int n) {
  // Create the buffers for communication.
  unsigned int *B0 = create_buffer();
  unsigned int *B1 = create_buffer();

  // Create the semaphors for communication.
  sem_t *sem0 = create_semaphor("/sem0");
  sem_t *sem1 = create_semaphor("/sem1");
  sem_t *sem2 = create_semaphor("/sem2");
  sem_t *sem3 = create_semaphor("/sem3");

  pid_t child_1_pid = checked_fork();
  if (child_1_pid != 0) {
    // Parent (p0)
    pid_t child_2_pid = checked_fork();
    if (child_2_pid != 0) {
      // Parent (p0)
      for (unsigned int i = 1; i <= n; ++i) {
        // Wait for P1 to flag for data.
        sem_wait(sem0);
        // Write the data.
        *B0 = i;
        // Flag to P1 that the data is written.
        sem_post(sem1);
      }

      // Pass in the sentinel to indicate termination.
      sem_wait(sem0);
      *B0 = sentinel;
      sem_post(sem1);

      // Wait for children to terminate.
      for (int i = 0; i < 2; i++) {
        waitpid(-1, NULL, 0);
      }
      /* cleanup */
      sem_close(sem0);
      sem_close(sem1);
      sem_close(sem2);
      sem_close(sem3);
      munmap(B0, sizeof(unsigned int));
      munmap(B1, sizeof(unsigned int));
      return;
    } else {
      // child_2 (p2)
      unsigned int value;

      do {
        // Flag to child_1 that the child_2 is waiting for data.
        sem_post(sem3);
        // Wait until child_1 has flagged that the data is written.
        sem_wait(sem2);
        // Read the data
        value = *B1;

        if (value != sentinel && value % 3 != 0) {
          printf("%u\n", value);
        }
      } while (value != sentinel);

      /* cleanup */
      sem_close(sem0);
      sem_close(sem1);
      sem_close(sem2);
      sem_close(sem3);
      return;
    }
  } else {
    // child_1 (p1)
    unsigned int value;
    do {
      // Flag to parent that the child_1 is waiting for data.
      sem_post(sem0);
      // Wait until parent has flagged that the data is written.
      sem_wait(sem1);
      // Read the data
      value = *B0;

      if (value % 2 != 0) {
        // Wait for child_2 to flag for data.
        sem_wait(sem3);
        // Write the data.
        *B1 = value;
        // Flag to child_2 that the data is written.
        sem_post(sem2);
      }
    } while (value != sentinel);

    // Pass in the sentinel to indicate termination.
    sem_wait(sem3);
    *B1 = 0;
    sem_post(sem2);

    /* cleanup */
    sem_close(sem0);
    sem_close(sem1);
    sem_close(sem2);
    sem_close(sem3);
    return;
  }
}

int main() {
  // Scan in the upper-bound.
  unsigned int n;
  scanf("%u", &n);
  execute_pipeline(n);

  return EXIT_SUCCESS;
}
