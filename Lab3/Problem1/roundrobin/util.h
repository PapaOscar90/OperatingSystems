#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include "process.h"
#include "scheduler.h"

void printArrays(int *array, int length)
{
    printf("[");
    for (int i = 0; i < length - 1; i++)
    {
        printf("%d,", array[i]);
    }
    printf("%d]", array[length - 1]);
}

// Debug printing of process
void printProcess(Process testProcess)
{
    printf("Process CPUq: ");
    printArrays(testProcess.CPUq, testProcess.sizeCPUq);
    printf("\nProcess IOq: ");
    printArrays(testProcess.IOq, testProcess.sizeIOq);
    printf("\n-------------------------------\n");
}

// Debug printing of a scheduler and it's contents
void printScheduler(Scheduler scheduler)
{
    printf("Scheduler (Q=%d) Contains:\n# Standby: %d\n", scheduler.quantum, scheduler.numStandby);
    printf("Priority1:\n---------------------");
    Process process;
    for (int i = 0; i < scheduler.numP1; i++)
    {
        process = scheduler.priority1[i];
        printProcess(process);
    }
}

#endif /* UTIL_H */