/*
 * File System Traversal
 * 
 * Troy Karan Harrison - s2841886
 * Philip Oetinger - s2966018
 * 
 * Traverses the file system starting from folder given in stdin. It compares the files for equality and prints all unique pairs that have the same content.
 * 
 */

/*
 * Pseudocode
 * M
 * 
 */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
    char *lineIn;
    size_t lineLen;

    lineLen = getline(&lineIn, &lineLen, stdin);
    lineIn[lineLen-1] = '\0';
    lineLen--;

    printf("%lu\n", lineLen);
    printf("%s\n", lineIn);
    return 0;
}