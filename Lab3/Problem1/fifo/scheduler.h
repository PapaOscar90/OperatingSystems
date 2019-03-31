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

  int totalNumberOfProcesses;
  int finishedProcesses;
  int totalTurnAroundTime;

  // A standby Q of inactive processes yet to begin
  int sizeStandby, numStandby;
  Process *standbyQ;

  int sizeActive, numActive;
  Process *activeProcesses;

} Scheduler;

// Creates a scheduler with a certain quantum that determines the whole rate
Scheduler createScheduler(int quantum)
{
  Scheduler newScheduler;
  newScheduler.quantum = quantum;
  newScheduler.currentTime = 0;
  newScheduler.sizeStandby = 20;
  newScheduler.numStandby = 0;
  newScheduler.sizeActive = 20;
  newScheduler.numActive = 0;

  newScheduler.finishedProcesses = 0;

  newScheduler.standbyQ = malloc(newScheduler.sizeStandby * sizeof(Process));
  newScheduler.activeProcesses = malloc(newScheduler.sizeActive * sizeof(Process));

  return newScheduler;
}

void addProcessToStandby(Scheduler *scheduler, Process *process)
{
  printf("Adding process to standby. num/size %d/%d\n", scheduler->numStandby, scheduler->sizeStandby);
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
      // Send the new process to the active Q
      if (scheduler->numActive == scheduler->sizeActive)
      {
        scheduler->sizeActive *= 2;
        scheduler->activeProcesses = realloc(scheduler->activeProcesses, scheduler->sizeActive * sizeof(Process));
      }
      scheduler->activeProcesses[scheduler->numActive] = scheduler->standbyQ[i];
      for (int j = i; j < scheduler->numStandby - 1; j++)
      {
        scheduler->standbyQ[j] = scheduler->standbyQ[j + 1];
      }
      scheduler->numStandby--;
      scheduler->numActive++;
    }
  }
}

void processRound(Scheduler *scheduler)
{
  updateActiveProcesses(scheduler);
  int cpuDone = 0;
  int ioDone = 0;

  for (int i = 0; i < scheduler->numActive; i++)
  {
    if (scheduler->activeProcesses[i].status == 0 && !cpuDone)
    {
      // Subtract CPU Time
      scheduler->currentTime = scheduler->currentTime + scheduler->activeProcesses[i].CPUq[scheduler->activeProcesses[i].frontCPUq];
      scheduler->activeProcesses[i].CPUq[scheduler->activeProcesses[i].frontCPUq] = 0;
      scheduler->activeProcesses[i].frontCPUq++;
      scheduler->activeProcesses[i].status++;
      cpuDone = 1;
    }

    if (scheduler->activeProcesses[i].status == 1 && !ioDone)
    {
      // Subtract io time
      scheduler->currentTime = scheduler->currentTime + scheduler->activeProcesses[i].IOq[scheduler->activeProcesses[i].frontIOq];
      scheduler->activeProcesses[i].IOq[scheduler->activeProcesses[i].frontIOq] = 0;
      scheduler->activeProcesses[i].frontCPUq++;
      scheduler->activeProcesses[i].status--;
      ioDone = 1;
    }

    if (scheduler->activeProcesses[i].frontCPUq == scheduler->activeProcesses[i].sizeCPUq && scheduler->activeProcesses[i].frontIOq == scheduler->activeProcesses[i].sizeIOq)
    {
      scheduler->activeProcesses[i].status = 3;
      scheduler->finishedProcesses++;
    }

    if (cpuDone && ioDone)
    {
      return;
    }
  }
}

#endif /* SCHEDULER_H */