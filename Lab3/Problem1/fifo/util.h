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
    printf("Process StartTime: %d, EndTime: %d\n", testProcess.startTime, testProcess.endTime);
    printf("Process CPUq: ");
    printArrays(testProcess.CPUq, testProcess.sizeCPUq);
    printf("\nProcess IOq: ");
    printArrays(testProcess.IOq, testProcess.sizeIOq);
    printf("\n-------------------------------\n");
}

// Debug printing of a scheduler and it's contents
void printScheduler(Scheduler scheduler)
{
    printf("\nScheduler (Q=%d) Contains:\n# Standby: %d\n", scheduler.quantum, scheduler.numStandby);
    printf("Active Processes: %d \n--------------------\n", scheduler.numActive);
    for (int i = 0; i < scheduler.numActive; i++)
    {
        printProcess(scheduler.activeProcesses[i]);
    }
}

#endif /* UTIL_H */