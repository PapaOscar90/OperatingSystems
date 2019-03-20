#include <stdio.h>
#include <stdlib.h>

#include "util.h"

typedef struct Scheduler
{
    int queue1;
} Scheduler;

typedef struct Process
{
    int priority;
    int aliveTime;
    int frontCPUq, frontIOq;
    int backCPUq, backIOq;
    int sizeCPUq;
    int sizeIOq;
    int *CPUq;
    int *IOq;
} Process;

// Add a CPU process to queue
void addCPUToProcess(Process *process, int cpuTime)
{
    // If there is no more room in the array
    if (process->backCPUq == process->sizeCPUq)
    {
        // Double the size of the array, and exit with failure if allocation fails
        process->sizeCPUq *= 2;
        process->CPUq = realloc(process->CPUq, process->sizeCPUq * sizeof(int));
        if (process->CPUq == NULL)
        {
            exit(EXIT_FAILURE);
        }
    }

    // Add the CPU time to the array, increment the back of q
    process->CPUq[process->backCPUq] = cpuTime;
    process->backCPUq++;
    return;
}

// Add an IO request to queue
void addIOToProcess(Process *process, int ioTime)
{
    // If there is no more room in the array
    if (process->backIOq == process->sizeIOq)
    {
        // Double the size of the array, and exit with failure if allocation fails
        process->sizeIOq *= 2;
        process->IOq = realloc(process->IOq, process->sizeIOq * sizeof(int));
        if (process->IOq == NULL)
        {
            exit(EXIT_FAILURE);
        }
    }

    // Add the IO time to the array, increment the back of q
    process->IOq[process->backIOq] = ioTime;
    process->backIOq++;
    return;
}

Process createProcess(int priority)
{
    Process newProcess;
    newProcess.priority = priority;
    newProcess.sizeCPUq = 20;
    newProcess.sizeIOq = 20;
    newProcess.aliveTime = 0;

    newProcess.CPUq = malloc(newProcess.sizeCPUq * sizeof(int));
    newProcess.IOq = malloc(newProcess.sizeIOq * sizeof(int));

    if (newProcess.CPUq == NULL || newProcess.IOq == NULL)
    {
        printf("Failed to allocate memory for new process\n");
        exit(EXIT_FAILURE);
    }

    newProcess.frontCPUq = 0;
    newProcess.frontIOq = 0;
    newProcess.backCPUq = 0;
    newProcess.backIOq = 0;

    return newProcess;
}

int main(int argc, char *argv[])
{
    printf("Hello world!\n");
    Process testProcess = createProcess(1);

    printf("Process Priority: %d\nCPUq Length: %d\nIOq Length: %d\n", testProcess.priority, testProcess.sizeCPUq, testProcess.sizeIOq);
    printf("Process CPUq: ");
    printArrays(testProcess.CPUq, testProcess.sizeCPUq);
    printf("\nProcess IOq: ");
    printArrays(testProcess.IOq, testProcess.sizeIOq);
    printf("\n");

    addCPUToProcess(&testProcess, 100);
    addCPUToProcess(&testProcess, 200);
    addCPUToProcess(&testProcess, 150);
    addCPUToProcess(&testProcess, 50);
    addCPUToProcess(&testProcess, 10);
    addCPUToProcess(&testProcess, 100);

    printf("Process CPUq: ");
    printArrays(testProcess.CPUq, testProcess.sizeCPUq);
    printf("\nProcess IOq: ");
    printArrays(testProcess.IOq, testProcess.sizeIOq);
    printf("\n");
}