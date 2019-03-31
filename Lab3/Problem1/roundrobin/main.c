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
    printf("Starting scheduler...\n");
    runScheduler(&scheduler);

    printf("\nFinal state of scheduler: \n");
    printScheduler(scheduler);
    return 0;
}
