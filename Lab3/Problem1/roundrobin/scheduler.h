#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdlib.h>
#include <string.h>

#include "process.h"

// Defines the scheduler and all it contains
typedef struct Scheduler
{
  // The max time a process has to run before interrupt
  int quantum;
  int currentTime;
  // Number of Processess
  int numP1, numP2, numP3;
  // Max size of priority Qs
  int sizeP1, sizeP2, sizeP3;
  // The active process in queue
  int activeP1, activeP2, activeP3;

  // Each priority Q
  Process *priority1;
  Process *priority2;
  Process *priority3;

  // A standby Q of inactive processes yet to begin
  int sizeStandby, numStandby;
  Process *standbyQ;

} Scheduler;

// Creates a scheduler with a certain quantum that determines the whole rate
Scheduler createScheduler(int quantum)
{
  Scheduler newScheduler;
  newScheduler.quantum = quantum;
  newScheduler.currentTime = 0;

  newScheduler.sizeP1 = 20;
  newScheduler.sizeP2 = 20;
  newScheduler.sizeP3 = 20;
  newScheduler.sizeStandby = 50;
  newScheduler.numP1 = 0;
  newScheduler.numP2 = 0;
  newScheduler.numP3 = 0;
  newScheduler.numStandby = 0;
  newScheduler.activeP1 = 0;
  newScheduler.activeP2 = 0;
  newScheduler.activeP3 = 0;

  newScheduler.priority1 = malloc(newScheduler.sizeP1 * sizeof(Process));
  newScheduler.priority2 = malloc(newScheduler.sizeP2 * sizeof(Process));
  newScheduler.priority3 = malloc(newScheduler.sizeP3 * sizeof(Process));
  newScheduler.standbyQ = malloc(newScheduler.sizeStandby * sizeof(Process));

  return newScheduler;
}

// Add a process to the Priority 1 Q
void addProcessToSchedulerP1(Scheduler *scheduler, int i)
{
  if (scheduler->numP1 == scheduler->sizeP1)
  {
    scheduler->sizeP1 *= 2;
    scheduler->priority1 = realloc(scheduler->priority1, scheduler->sizeP1 * sizeof(Process));
  }
  scheduler->priority1[scheduler->numP1] = scheduler->standbyQ[i];
  for (int j = i; j < scheduler->numStandby - 1; j++)
  {
    scheduler->standbyQ[j] = scheduler->standbyQ[j + 1];
  }
  scheduler->numStandby--;
  scheduler->numP1++;
}

// Add a process to the Priority 2 Q
void addProcessToSchedulerP2(Scheduler *scheduler, int i)
{
  if (scheduler->numP2 == scheduler->sizeP2)
  {
    scheduler->sizeP2 *= 2;
    scheduler->priority2 = realloc(scheduler->priority2, scheduler->sizeP2 * sizeof(Process));
  }
  scheduler->priority2[scheduler->numP2] = scheduler->standbyQ[i];
  for (int j = i; j < scheduler->numStandby - 1; j++)
  {
    scheduler->standbyQ[j] = scheduler->standbyQ[j + 1];
  }
  scheduler->numStandby--;
  scheduler->numP2++;
}

// Add a process to the Priority 3 Q
void addProcessToSchedulerP3(Scheduler *scheduler, int i)
{
  if (scheduler->numP3 == scheduler->sizeP3)
  {
    scheduler->sizeP3 *= 2;
    scheduler->priority3 = realloc(scheduler->priority3, scheduler->sizeP3 * sizeof(Process));
  }
  scheduler->priority3[scheduler->numP3] = scheduler->standbyQ[i];
  for (int j = i; j < scheduler->numStandby - 1; j++)
  {
    scheduler->standbyQ[j] = scheduler->standbyQ[j + 1];
  }
  scheduler->numStandby--;
  scheduler->numP3++;
}

void addProcessToStandby(Scheduler *scheduler, Process *process)
{
  if (scheduler->numStandby == scheduler->sizeStandby)
  {
    scheduler->sizeStandby *= 2;
    scheduler->standbyQ = realloc(scheduler->standbyQ, scheduler->sizeStandby * sizeof(Process));
  }
  scheduler->standbyQ[scheduler->numStandby] = *process;
  scheduler->numStandby++;
}

// This will initialize and setup the scheduler by reading in all processes from stdin
void setupScheduler(Scheduler *scheduler)
{
  char *line = NULL;
  char *token;
  size_t len = 0;

  // While there is stuff to read...
  while (getline(&line, &len, stdin) != -1)
  {
    int startTime;
    int priority;
    int cpuRead, ioRead;

    // Use tokens to parse the lines and find each int
    token = strtok(line, " ");
    startTime = atoi(token);
    token = strtok(NULL, " ");
    priority = atoi(token);
    printf("Adding a process, priority %d, starting at %d...\n", priority, startTime);
    Process newProcess = createProcess(priority, startTime);

    while (1)
    {
      token = strtok(NULL, " ");
      cpuRead = atoi(token);

      // If there is nothing left to read break
      if (cpuRead != -1)
      {
        addCPUToProcess(&newProcess, cpuRead);
        token = strtok(NULL, " ");
        ioRead = atoi(token);

        // If there is nothing left to read break
        if (ioRead != -1)
        {
          addIOToProcess(&newProcess, ioRead);
          continue;
        }
      }
      break; // Stop reading in this process
    }
    addProcessToStandby(scheduler, &newProcess);
  }

  // Free memory
  free(line);
}

void updateActiveProcesses(Scheduler *scheduler)
{
  for (int i = 0; i < scheduler->numStandby; i++)
  {
    if (scheduler->standbyQ[i].startTime <= scheduler->currentTime)
    {
      // Send the new process to the correct Q based on priority
      switch (scheduler->standbyQ[i].priority)
      {
      case 1:
        addProcessToSchedulerP1(scheduler, i);
        i--;
        break;
      case 2:
        addProcessToSchedulerP2(scheduler, i);
        i--;
        break;
      case 3:
        addProcessToSchedulerP3(scheduler, i);
        i--;
        break;
      default:
        break;
      }
    }
  }
}

void runScheduler(Scheduler *scheduler)
{
  int currentQueue = 0;
  int activeProcessID = 0;
  Process *activeQueue;

  // Each round, add any newely added process (of current time) to queues
  updateActiveProcesses(scheduler);

  // Take the quantum from the first, second, or third priority depending on cycle
  switch (currentQueue)
  {
  case 0:
    activeQueue = scheduler->priority1;
    activeProcessID = scheduler->activeP1;
    break;
  case 1:
    activeQueue = scheduler->priority2;
    activeProcessID = scheduler->activeP2;
    break;
  case 2:
    activeQueue = scheduler->priority3;
    activeProcessID = scheduler->activeP3;
    break;
  default:
    currentQueue = (currentQueue + 1) % 3;
    break;
  }

  printf("Using Priority%d for this round. Active process = %d\n", currentQueue, activeProcessID);
}

#endif /* SCHEDULER_H */