![image](https://github.com/user-attachments/assets/97f5b61a-3a43-4ef7-911b-ca03d9a5f9ed)
![image](https://github.com/user-attachments/assets/1dcfea6a-36e9-49f6-bdf0-5a1a47a4e426)

<pre>
##This code implements three CPU scheduling algorithms:

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

##
The test cases evaluate FCFS, SJF, and RR under different scenarios. Below is a breakdown of each test case, explaining inputs, expected outputs, and which algorithm performs best in each scenario.


															
Test Case 1: Three Processes with Increasing Burst Times
Input
Process	Arrival	Burst
P1  0	10  
P2	1	5  
P3	2	8  
Results
Algorithm	Turnaround	Waiting	Response
FCFS	15.00	7.33	7.33
SJF	15.00	7.33	7.33
RR (Q=4)	19.33	11.67	3.00
Analysis
FCFS & SJF perform identically because the shortest job (P2) arrives after a longer job (P1), so SJF cannot optimize.

RR has higher turnaround due to context switching but better response time (3.00 vs 7.33).

Best Algorithm:

For response time: RR (best for interactivity).

For turnaround/waiting: FCFS/SJF (same here, but SJF usually better when short jobs arrive early).


Test Case 2: Processes with Gaps (Idle CPU)
Input
Process	Arrival	Burst
P1	2	6
P2	5	2
P3	8	4
Results
Algorithm	Turnaround	Waiting	Response
FCFS	5.67	1.67	1.67
SJF	5.67	1.67	1.67
RR (Q=2)	5.67	1.67	1.00
Analysis
All algorithms perform similarly because processes do not overlap (no contention).

RR still has slightly better response time (1.00 vs 1.67).

Best Algorithm:

RR (better response, same turnaround).


	
	

Test Case 3: Short Job Arrives Later (SJF Advantage)
Input
Process	Arrival	Burst
P1	0	4
P2	1	3
P3	2	1
P4	3	2
Results
Algorithm	Turnaround	Waiting	Response
FCFS	5.75	3.25	3.25
SJF	5.00	2.50	2.50
RR (Q=2)	6.25	3.75	1.75
Analysis
SJF outperforms FCFS because it picks the shortest job (P3) when it arrives at t=2.

RR has worse turnaround due to preemption but best response time (1.75).

Best Algorithm:

For turnaround/waiting: SJF (optimal for batch processing).

For response time: RR (best for interactive systems).


	
	
	
	
Test Case 4: Mixed Burst Times with Overlapping Arrivals
Input
Process	Arrival	Burst
P1	0	3
P2	2	6
P3	4	4
P4	6	5
P5	8	2
Results
Algorithm	Turnaround	Waiting	Response
FCFS	8.60	4.60	4.60
SJF	7.60	3.60	3.60
RR (Q=3)	10.60	6.60	2.60
Analysis
SJF beats FCFS because it prioritizes shorter jobs (e.g., P5 with burst=2).

RR has worst turnaround but best response time.

Best Algorithm:

For minimizing waiting time: SJF.

For fairness/response: RR.


	
	
	

Test Case 5: Single Process (Edge Case)
Input
Process	Arrival	Burst
P1	0	5
Results
Algorithm	Turnaround	Waiting	Response
All	5.00	0.00	0.00
Analysis
All algorithms perform the same (no scheduling needed).

Best Algorithm:

Any (no difference).


	
	
	
Test Case 6: Two Processes, Same Arrival Time
Input
Process	Arrival	Burst
P1	0	4
P2	0	6
Results
Algorithm	Turnaround	Waiting	Response
FCFS/SJF	7.00	2.00	2.00
RR (Q=3)	8.50	3.50	1.50
Analysis
FCFS & SJF behave the same (since both arrive at the same time).

RR has higher turnaround due to preemption but better response time.

Best Algorithm:

For throughput: FCFS/SJF.

For fairness: RR.


	
	
Test Case 7: Varying Arrivals with Long & Short Jobs
Input
Process	Arrival	Burst
P1	0	8
P2	1	4
P3	2	9
P4	3	5
Results
Algorithm	Turnaround	Waiting	Response
FCFS	15.25	8.75	8.75
SJF	14.25	7.75	7.75
RR (Q=3)	20.00	13.50	3.00
Analysis
SJF is better than FCFS because it prioritizes shorter jobs (P2, P4).

RR has worst turnaround but best response time.

Best Algorithm:

For batch processing: SJF.

For interactive systems: RR.


	
	
	
	
	
Test Case 8: Six Processes with Random Arrivals
Input
Process	Arrival	Burst
P1	0	10
P2	2	3
P3	4	7
P4	6	5
P5	7	2
P6	9	8
Results
Algorithm	Turnaround	Waiting	Response
FCFS	17.00	11.17	11.17
SJF	15.17	9.33	9.33
RR (Q=4)	20.67	14.83	6.33




	
Analysis
SJF is best for minimizing waiting time.

RR has worst turnaround but better response.

Best Algorithm:

For batch jobs: SJF.

For time-sharing: RR.

Final Summary:

Short jobs arrive late	FCFS/SJF (same)	No optimization possible
Idle CPU periods	RR (best response)	No contention, but RR still fair
Short jobs arrive early	SJF	Minimizes waiting time
Interactive systems	RR	Best response time
Single process	Any	No scheduling needed
Same arrival time	FCFS/SJF (same)	No SJF optimization
Mixed long & short jobs	SJF	Better waiting time
Complex workloads	SJF (batch) / RR (interactive)	Depends on goal
Key Takeaways
FCFS is simple but bad for short jobs if they arrive after long ones.

SJF is optimal for minimizing waiting time but requires knowing burst times.

RR is best for response time (interactive systems) but has higher overhead.

The test cases validate that each algorithm behaves as expected under different workloads
#
</pre>
