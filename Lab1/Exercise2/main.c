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

  return EXIT_SUCCESS;
}
