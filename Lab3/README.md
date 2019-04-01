# Operating Systems Report
Troy Harrison & Philip Oetinger

# Problem 1
For both of the versions of problem one, we built a simulation of a scheduler that would compute every step of the process. The main data structures include the scheduler itself, and processes. The processes contain information about when they start, what cpu tasks they need, IO tasks, and how long they were alive for. We store the processes in a master array that we can index and place each processes in the active part of the scheduler. 

When a process is activated, it gets put into the active queue. Here it will wait for CPU time that is allocated by the scheduler. Once it blocks (for IO), it then gets put in the queue for IO requests. Since only one device exists, it operates in a similar fashion to the CPU resource. Whereby only one process may gain access at a time, and holds a lockdown on it until it is finished. When a process is finished with the IO device, it is then passed back to the scheduler where it is put at the end of the queue again, if it needs CPU time.

This whole process repeats over and over until there in nothing left requesting CPU resources and IO. At this point the scheduler can tally up the times from each process, and then compute the average turnaround time. 

## FIFO Details

### Results

## RoundRobin Details

### Results


# Problem 2
The second problem was very straightforward, and only involved a short algorithm to replicate the page fault process.

## FIFO Details
Just as with a real FIFO system, the first page that is loaded is also the first page to be removed from the active frames when a new page needs to be loaded. This causes alot of re-loading, and a major slowdown in performance.

### Results
(insert examples here)

## clock Details

### Results
