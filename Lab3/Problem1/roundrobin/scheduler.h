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
  // Number of Processess
  int numP1, numP2, numP3;
  // Max size of priority Qs
  int sizeP1, sizeP2, sizeP3;

  // Each priority Q
  Process *priority1;
  Process *priority2;
  Process *priority3;
} Scheduler;

Scheduler createScheduler(int quantum);
void addProcessToSchedulerP1(Scheduler *scheduler, Process *process);

// Creates a scheduler with a certain quantum that determines the whole rate
Scheduler createScheduler(int quantum)
{
  Scheduler newScheduler;
  newScheduler.quantum = quantum;

  newScheduler.sizeP1 = 20;
  newScheduler.sizeP2 = 20;
  newScheduler.sizeP3 = 20;
  newScheduler.numP1 = 0;
  newScheduler.numP2 = 0;
  newScheduler.numP3 = 0;

  newScheduler.priority1 = malloc(newScheduler.sizeP1 * sizeof(Process));
  newScheduler.priority2 = malloc(newScheduler.sizeP2 * sizeof(Process));
  newScheduler.priority3 = malloc(newScheduler.sizeP3 * sizeof(Process));

  return newScheduler;
}

// Add a process to the Priority 1 Q
void addProcessToSchedulerP1(Scheduler *scheduler, Process *process)
{
  if (scheduler->numP1 == scheduler->sizeP1)
  {
    scheduler->sizeP1 *= 2;
    scheduler->priority1 = realloc(scheduler->priority1, scheduler->sizeP1 * sizeof(Process));
  }
  scheduler->priority1[scheduler->numP1] = *process;
  scheduler->numP1++;
}

// Add a process to the Priority 2 Q
void addProcessToSchedulerP2(Scheduler *scheduler, Process *process)
{
  if (scheduler->numP2 == scheduler->sizeP2)
  {
    scheduler->sizeP2 *= 2;
    scheduler->priority2 = realloc(scheduler->priority2, scheduler->sizeP2 * sizeof(Process));
  }
  scheduler->priority2[scheduler->numP2] = *process;
  scheduler->numP2++;
}

// Add a process to the Priority 3 Q
void addProcessToSchedulerP3(Scheduler *scheduler, Process *process)
{
  if (scheduler->numP3 == scheduler->sizeP3)
  {
    scheduler->sizeP3 *= 2;
    scheduler->priority3 = realloc(scheduler->priority3, scheduler->sizeP3 * sizeof(Process));
  }
  scheduler->priority3[scheduler->numP3] = *process;
  scheduler->numP3++;
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

    // Send the new process to the correct Q based on priority
    switch (priority)
    {
    case 1:
      addProcessToSchedulerP1(scheduler, &newProcess);
      break;
    case 2:
      addProcessToSchedulerP2(scheduler, &newProcess);
      break;
    case 3:
      addProcessToSchedulerP3(scheduler, &newProcess);
      break;
    default:
      break;
    }
  }

  // Free memory
  free(line);
}

#endif /* SCHEDULER_H */