![image](https://github.com/user-attachments/assets/97f5b61a-3a43-4ef7-911b-ca03d9a5f9ed)![image](https://github.com/user-attachments/assets/a7d3c362-9d7a-44aa-85b2-9174c901cfe0)
![image](https://github.com/user-attachments/assets/1dcfea6a-36e9-49f6-bdf0-5a1a47a4e426)


This code implements three CPU scheduling algorithms:

First-Come-First-Served (FCFS)

Shortest Job First (SJF) (Non-Preemptive)

Round Robin (RR)

Process Structure (Process)
Each process has:

pid: Process ID (not used in calculations but useful for tracking)

arrivalTime: When the process arrives in the ready queue

burstTime: Total CPU time required

remainingTime: Used in RR to track remaining execution time

startTime: When the process first gets CPU (for response time)

completionTime: When the process finishes execution (for turnaround time)

Metrics Structure (Metrics)
Stores average performance metrics:

avgTurnaround: (Completion Time – Arrival Time) averaged

avgWaiting: (Turnaround Time – Burst Time) averaged

avgResponse: (Start Time – Arrival Time) averaged

2. First-Come-First-Served (FCFS)
AlgorithmL
Processes are executed in order of arrival.

Non-preemptive: Once a process starts, it runs to completion.

Simple but can lead to convoy effect (long processes block short ones).

Variables & Logic:
currentTime: Tracks simulation time.

Processes are processed in input order (assumed sorted by arrival time).

If CPU is idle (currentTime < arrivalTime), time jumps to the next arrival.

Metrics are computed as:

Turnaround Time = completionTime - arrivalTime

Waiting Time = startTime - arrivalTime (same as response time in FCFS)


3. Shortest Job First (SJF) (Non-Preemptive)
Algorithm
Always picks the shortest available job (based on burst time).

Non-preemptive: Once a job starts, it runs to completion.

Optimal for minimizing average waiting time but requires knowing burst times.

Key Variables & Logic:
completed_proc[]: Tracks which processes have finished.

shortest: Index of the process with the shortest burst time among arrived processes.

shortest_burst: Helps in finding the minimum burst time.


Why SJF Beats FCFS in Some Cases
If a short job arrives while a long one is running, SJF schedules it earlier, reducing average waiting time.

4. Round Robin (RR)
Algorithm
Each process gets a fixed time quantum.

If not finished, it goes back to the end of the queue.

Preemptive: Ensures fairness but has overhead due to context switching.

Key Data Structures
inQueue[]: Boolean array tracking which processes are in the ready queue.

queue[]: Circular queue storing process indices.

front, rear: Manage the queue.

Functions
enqueue(): Adds a process to the queue.

dequeue(): Removes a process from the queue.


Initialize all processes with remainingTime = burstTime;

Why RR Has Higher Turnaround But Better Response Time
Turnaround Time ↑: Due to frequent context switches.

Response Time ↓: Every process gets CPU quickly (good for interactivity).

