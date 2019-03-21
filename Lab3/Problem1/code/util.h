#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>

void printArrays(int *array, int length)
{
    printf("[");
    for (int i = 0; i < length - 1; i++)
    {
        printf("%d,", array[i]);
    }
    printf("%d]", array[length - 1]);
}

#endif /* UTIL_H */