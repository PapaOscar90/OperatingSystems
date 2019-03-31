#include <stdio.h>
#include <stdlib.h>

#include "util.h"
#include "process.h"
#include "scheduler.h"

int main(int argc, char *argv[])
{
    Scheduler scheduler = createScheduler(10);
    setupScheduler(&scheduler);
    printf("NEW SCHEDULER CREATED\n");
    printScheduler(scheduler);
    printf("\n\n");

    scheduler.currentTime = 0;
    scheduler.totalNumberOfProcesses = scheduler.numStandby;
    scheduler.totalTurnAroundTime = 0;

    printf("Starting scheduler...\n");

    processRound(&scheduler);
    printScheduler(scheduler);

    processRound(&scheduler);
    printScheduler(scheduler);

    while (1)
    {
        processRound(&scheduler);

        if (scheduler.numActive == 0)
        {
            printf("Total Turnaround: %d\nAverage Tur-around: %\n", scheduler.totalTurnAroundTime, scheduler.totalTurnAroundTime / scheduler.totalNumberOfProcesses);
            break;
        }
    }
    return 0;
}
