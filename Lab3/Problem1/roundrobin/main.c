#include <stdio.h>
#include <stdlib.h>

#include "util.h"
#include "process.h"
#include "scheduler.h"

int main(int argc, char *argv[])
{
    Scheduler scheduler = createScheduler(10);
    setupScheduler(&scheduler);

    printScheduler(scheduler);

    scheduler.currentTime = 0;

    updateActiveProcesses(&scheduler);

    printScheduler(scheduler);
    return 0;
}
