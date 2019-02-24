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
  int parentID, childID;
  char msg[64];

  // Store the original processID for the final child to communicate with
  origParentID = getpid();
  int *fd[2];
  for(int i=0;i<5;i++){
    fd[i] = malloc(sizeof(int*));
  }

  int numberOfChildrenToMake = 5;
  int myRelativeID = 0;

  printf("Hello, I am the super_parent. My ID is %d, and RID is %d\n", getpid(), myRelativeID);

  while(1 && numberOfChildrenToMake > 0){
    int fd_child[2];

    childID = fork();
    if(childID != 0){
      // Parent runs this
      printf("%d created ID: %d\n", myRelativeID, childID);
      break;
    }else{
      myRelativeID++;
      numberOfChildrenToMake--;
      printf("Hello World, I am RID, %d (%d).\n", getpid(), myRelativeID);
      close(fd[myRelativeID][0]);
      sprintf(msg, "HelloThere.\n");
      write(fd[myRelativeID][1],msg,64);
      close(fd[myRelativeID][1]);
    }
  }

  if(myRelativeID == 5){
    printf("I'm the last child. I'm going to read, then ending.\n");
    return EXIT_SUCCESS;
  }else{
    waitpid(childID, &status,0);
    printf("My child, %d, ended, so I will end.\n", childID);
  }
  return EXIT_SUCCESS;
}
