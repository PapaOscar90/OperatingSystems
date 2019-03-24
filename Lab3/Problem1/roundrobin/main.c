#include <stdio.h>
#include <stdlib.h>

#include "util.h"
#include "process.h"
#include "scheduler.h"

int main(int argc, char *argv[])
{
    printf("Hello world!\n");
    Process testProcess = createProcess(1);

    printf("Process Priority: %d\nCPUq Length: %d\nIOq Length: %d\n", testProcess.priority, testProcess.sizeCPUq, testProcess.sizeIOq);

    printProcess(testProcess);

    addCPUToProcess(&testProcess, 100);
    addCPUToProcess(&testProcess, 200);
    addCPUToProcess(&testProcess, 150);
    addCPUToProcess(&testProcess, 50);
    addCPUToProcess(&testProcess, 10);
    addCPUToProcess(&testProcess, 100);

    printProcess(testProcess);

    Scheduler testScheduler = createScheduler(10);
    printScheduler(testScheduler);
    addProcessToSchedulerP1(&testScheduler, &testProcess);
    printScheduler(testScheduler);

    setupScheduler(testScheduler);
    return 0;
}
