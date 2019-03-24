#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/* This program demonstrates fork()
*/
int main(int argc, char *argv[]){
    int parent, child, status;

    parent = getpid();

    // Create a seperate PROCESS from this point, with the variables existing as is
    // until this point
    child = fork();

    // fork returns 0 for the child, or -1 if error
    if (child < 0 ){
        fprintf(stderr, "Fork failed: abort\n");
        return EXIT_FAILURE;
    }

    if (child != 0){
        /* Parent runs this*/
        printf("Parent: Mypid = %d, child = %d\n", parent, child);
    } else {
        /* Child runs this */
        printf( "Child: Mypid = %d, child = %d, parent = %d\n", child, parent, getppid());
        exit(99);
    }

    /*
     * ALL run this
     */
    waitpid(-1, &status, 0);
    printf("Status = %d, %d\n", status/256, status%256);
    return EXIT_SUCCESS;
}
