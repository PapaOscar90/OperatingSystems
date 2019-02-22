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


int main(){
  int origParentID, status;

  // Store the original processID for the final child to communicate with
  origParentID = getpid();
  int fdOrignal[2];

  int numberOfChildrenToMake = 5;
  int myRelativeID = 0;

  int iSpawnedAChild = 0;

  while(numberOfChildrenToMake > 0 && iSpawnedAChild == 0){
    int fdForChild[2];
    printf("Forking a new child...\n");
    int parentPID = getpid();
    int child = fork();

    if(child !=0){
      // Parent runs this
      iSpawnedAChild = 1;
    } else {
      // Child runs this
      numberOfChildrenToMake--;
      myRelativeID++;
      printf("I'm a new child id: %d. My parent is PID #%d\n", getpid(), parentPID);
      printf("numberOfChildrenToMake= %d, myRelativeID= %d\n", numberOfChildrenToMake, myRelativeID);
    }

  }

  // The parent needs to start counting, and then wait until it hears from the last child. There will be a message on the fdOrignal
  for(int i=0; i<5; i++){
    waitpid(-1, &status, 0);
  }

  printf("My ID is: %d. The master is: %d\n", getpid(), origParentID);
  if(getpid() == origParentID){
    /* close(fdOrignal[1]); */
    /* char msg[64]; */

    /* read(fd[0], msg, 64); */

    /* printf("I got: %s\n", msg); */

    /* close(fdOrignal[0]); */
    printf("I'm the originalParent. Waiting...\n");
    waitpid(getpid+5, &status, 0);
  }else{
    printf("This is the end. I'm pushing to my neighbor here.I am relativeID: %d\n", myRelativeID);
    if(myRelativeID == 5){
      /* close(fdOrignal[0]); */
      /* char msg[64]; */

      /* sprintf("helloParent", msg); */
      /* write(fdOrignal[1], msg, 64); */

      /* close(fdOrignal[1]); */

      printf("I was relativeID: %d, I sent a message to origanlParent.\n", myRelativeID);
    }
  }
  return EXIT_SUCCESS;
}
