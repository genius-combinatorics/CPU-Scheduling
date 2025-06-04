#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "scheduler.h"
#include <stdbool.h>
// FCFS Scheduling Algorithm
Metrics fcfs_metrics(Process proc[], int n) {
    int totalTurnaround = 0, totalWaiting = 0, totalResponse = 0;
    int currentTime = 0;

    for (int i = 0; i < n; i++) {
        if (currentTime < proc[i].arrivalTime) {
            currentTime = proc[i].arrivalTime;
        }
        proc[i].startTime = currentTime;
        proc[i].completionTime = currentTime + proc[i].burstTime;
        totalTurnaround += proc[i].completionTime - proc[i].arrivalTime;
        totalWaiting += proc[i].startTime - proc[i].arrivalTime;
        totalResponse += proc[i].startTime - proc[i].arrivalTime;

        currentTime = proc[i].completionTime;
    }

    Metrics metrics = {
        .avgTurnaround = (float)totalTurnaround / n,
        .avgWaiting = (float)totalWaiting / n,
        .avgResponse = (float)totalResponse / n
    };

    return metrics;
}

// SJF Scheduling Algorithm (Non-preemptive)
// SJF Scheduling Algorithm (Non-preemptive)
// SJF Scheduling Algorithm (Non-preemptive)
Metrics sjf_metrics(Process proc[], int n) {
    int totalTurnaround = 0, totalWaiting = 0, totalResponse = 0;
    int currentTime = 0;
    int completed = 0;
    bool completed_proc[n];

    // Initialize all processes as not completed
    for (int i = 0; i < n; i++) {
        completed_proc[i] = false;
    }

    while (completed < n) {
        int shortest = -1;
        int shortest_burst = INT_MAX;

        // Find the process with shortest burst time among arrived processes
        for (int i = 0; i < n; i++) {
            if (!completed_proc[i] && proc[i].arrivalTime <= currentTime && proc[i].burstTime < shortest_burst) {
                shortest_burst = proc[i].burstTime;
                shortest = i;
            }
        }

        if (shortest == -1) {
            // No process available, advance time to next arrival
            int next_arrival = INT_MAX;
            for (int i = 0; i < n; i++) {
                if (!completed_proc[i] && proc[i].arrivalTime < next_arrival) {
                    next_arrival = proc[i].arrivalTime;
                }
            }
            currentTime = next_arrival;
            continue;
        }

        // Execute the shortest job
        proc[shortest].startTime = currentTime;
        proc[shortest].completionTime = currentTime + proc[shortest].burstTime;
        totalTurnaround += proc[shortest].completionTime - proc[shortest].arrivalTime;
        totalWaiting += proc[shortest].startTime - proc[shortest].arrivalTime;
        totalResponse += proc[shortest].startTime - proc[shortest].arrivalTime;

        currentTime = proc[shortest].completionTime;
        completed_proc[shortest] = true;
        completed++;
    }

    Metrics metrics = {
        .avgTurnaround = (float)totalTurnaround / n,
        .avgWaiting = (float)totalWaiting / n,
        .avgResponse = (float)totalResponse / n
    };

    return metrics;
}

// Round Robin Scheduling Algorithm
Metrics rr_metrics(Process proc[], int n, int timeQuantum) {
    int totalTurnaround = 0, totalWaiting = 0, totalResponse = 0;
    int currentTime = 0, remainingProcesses = n;
    
    // Track which processes are in the queue
    bool inQueue[n];
    
    // Initialize remaining time, startTime, and queue status
    for (int j = 0; j < n; j++) {
        proc[j].remainingTime = proc[j].burstTime;
        proc[j].startTime = -1;  // Mark as uninitialized
        inQueue[j] = false;      // Track if process is in queue
    }
    
    // Create a circular queue
    int queue[n];
    int front = -1, rear = -1;
    
    // Function to enqueue a process
    void enqueue(int processIndex) {
        if ((rear + 1) % n == front) return; // Queue is full
        if (front == -1) front = 0;
        rear = (rear + 1) % n;
        queue[rear] = processIndex;
        inQueue[processIndex] = true;
    }
    
    // Function to dequeue a process
    int dequeue() {
        if (front == -1) return -1; // Queue is empty
        int processIndex = queue[front];
        if (front == rear) {
            front = rear = -1;
        } else {
            front = (front + 1) % n;
        }
        inQueue[processIndex] = false;
        return processIndex;
    }
    
    // Enqueue processes that have arrived by currentTime
    for (int j = 0; j < n; j++) {
        if (proc[j].arrivalTime <= currentTime && !inQueue[j] && proc[j].remainingTime > 0) {
            enqueue(j);
        }
    }
    
    // Round Robin Execution Loop
    while (remainingProcesses > 0) {
        if (front == -1) {
            // No processes ready, advance time to next arrival
            int nextArrival = INT_MAX;
            for (int j = 0; j < n; j++) {
                if (proc[j].remainingTime > 0 && proc[j].arrivalTime < nextArrival && !inQueue[j]) {
                    nextArrival = proc[j].arrivalTime;
                }
            }
            if (nextArrival != INT_MAX) {
                currentTime = nextArrival;
                // Enqueue processes that have arrived by new currentTime
                for (int j = 0; j < n; j++) {
                    if (proc[j].arrivalTime <= currentTime && proc[j].remainingTime > 0 && !inQueue[j]) {
                        enqueue(j);
                    }
                }
            }
            continue;
        }
        
        int currentProcess = dequeue();
        if (currentProcess == -1) break;
        
        // Set startTime only the first time the process is scheduled
        if (proc[currentProcess].startTime == -1) {
            proc[currentProcess].startTime = currentTime;
        }
        
        // Execute the process for the time quantum or the remaining time
        int executionTime = (proc[currentProcess].remainingTime > timeQuantum) ? timeQuantum : proc[currentProcess].remainingTime;
        proc[currentProcess].remainingTime -= executionTime;
        currentTime += executionTime;
        
        // Check for new arrivals during this execution
        for (int j = 0; j < n; j++) {
            if (proc[j].arrivalTime <= currentTime && proc[j].remainingTime > 0 && !inQueue[j] && j != currentProcess) {
                enqueue(j);
            }
        }
        
        // If the process hasn't finished, put it back in the queue
        if (proc[currentProcess].remainingTime > 0) {
            enqueue(currentProcess);
        } else {
            // Process finished, calculate metrics
            proc[currentProcess].completionTime = currentTime;
            totalTurnaround += proc[currentProcess].completionTime - proc[currentProcess].arrivalTime;
            totalWaiting += proc[currentProcess].completionTime - proc[currentProcess].arrivalTime - proc[currentProcess].burstTime;
            totalResponse += proc[currentProcess].startTime - proc[currentProcess].arrivalTime;
            remainingProcesses--;
        }
    }
    
    // Calculate the average metrics
    Metrics metrics = {
        .avgTurnaround = (float)totalTurnaround / n,
        .avgWaiting = (float)totalWaiting / n,
        .avgResponse = (float)totalResponse / n
    };
    
    return metrics;
}
