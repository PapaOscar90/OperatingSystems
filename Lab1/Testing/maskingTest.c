#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>

// This will create our own sig handler
void handler(int sig) {
  printf("\n\nhandler: caught signal %d\n", sig);
  if (sig == SIGINT) {
    printf("See you next time\n");
    exit(EXIT_SUCCESS);
  }
  return;
}

int main(int argc, char *argv[]) {
  struct sigaction newsigact;
  sigset_t sset;

  newsigact.sa_handler = handler;
  sigemptyset(&newsigact.sa_mask);
  newsigact.sa_flags = 0;

  // Set the new handler for SIGALRM and SIGINT
  sigaction(SIGALRM, &newsigact, NULL);
  sigaction(SIGINT, &newsigact, NULL);
  
  alarm(1); /* send in 1 second a SIGALRM */ /* mask alarm signal */
  
  // Empty sset, then add in SIGALRM so it is able to be blocked
  sigemptyset(&sset);
  sigaddset(&sset, SIGALRM);

  // This then blocks EVERYTHING in sset
  sigprocmask(SIG_BLOCK, &sset, NULL);

  int counter = 1;
  while (counter < INT_MAX/2) {
      counter *= 2;
  }

  // This unblocks everything in sset
  sigprocmask(SIG_UNBLOCK, &sset, NULL);
  
  while(1){

  }
  return EXIT_SUCCESS;
}