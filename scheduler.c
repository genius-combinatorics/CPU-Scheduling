#include <stdio.h>
#include <stdlib.h>
#include "scheduler.h"

// Helper function to sort processes by arrival time (for FCFS) and burst time (for SJF)
int compareArrivalTime(const void *a, const void *b) {
    return ((Process*)a)->arrivalTime - ((Process*)b)->arrivalTime;
}

int compareBurstTime(const void *a, const void *b) {
    return ((Process*)a)->burstTime - ((Process*)b)->burstTime;
}

// Function to calculate metrics (Turnaround, Waiting, Response) for FCFS
Metrics calculate_metrics(Process proc[], int n) {
    Metrics metrics = {0, 0, 0};
    int totalTurnaround = 0, totalWaiting = 0, totalResponse = 0;

    for (int i = 0; i < n; i++) {
        int turnaroundTime = proc[i].completionTime - proc[i].arrivalTime;
        int waitingTime = turnaroundTime - proc[i].burstTime;
        int responseTime = proc[i].startTime - proc[i].arrivalTime;

        totalTurnaround += turnaroundTime;
        totalWaiting += waitingTime;
        totalResponse += responseTime;
    }

    // Calculate average metrics
    metrics.avgTurnaround = (float)totalTurnaround / n;
    metrics.avgWaiting = (float)totalWaiting / n;
    metrics.avgResponse = (float)totalResponse / n;

    return metrics;
}

// FCFS Scheduling Algorithm
Metrics fcfs_metrics(Process proc[], int n) {
    Metrics metrics = {0, 0, 0};
    int currentTime = 0; // Tracks the current time

    // Sort processes by arrival time
    qsort(proc, n, sizeof(Process), compareArrivalTime);

    // Simulate the execution of processes
    for (int i = 0; i < n; i++) {
        // Adjust current time to the arrival time of the process if needed
        if (proc[i].arrivalTime > currentTime) {
            currentTime = proc[i].arrivalTime;
        }

        // Set the start and completion times for each process
        proc[i].startTime = currentTime;
        proc[i].completionTime = currentTime + proc[i].burstTime;

        // Move the current time forward
        currentTime = proc[i].completionTime;
    }

    // Calculate and return the metrics
    return calculate_metrics(proc, n);
}

// SJF Scheduling Algorithm (Non-preemptive)
Metrics sjf_metrics(Process proc[], int n) {
    Metrics metrics = {0, 0, 0};
    int currentTime = 0; // Tracks the current time

    // Sort processes by burst time
    qsort(proc, n, sizeof(Process), compareBurstTime);

    // Simulate the execution of processes
    for (int i = 0; i < n; i++) {
        // Adjust current time to the arrival time of the process if needed
        if (proc[i].arrivalTime > currentTime) {
            currentTime = proc[i].arrivalTime;
        }

        // Set the start and completion times for each process
        proc[i].startTime = currentTime;
        proc[i].completionTime = currentTime + proc[i].burstTime;

        // Move the current time forward
        currentTime = proc[i].completionTime;
    }

    // Calculate and return the metrics
    return calculate_metrics(proc, n);
}

// Round Robin Scheduling Algorithm
Metrics rr_metrics(Process proc[], int n, int timeQuantum) {
    Metrics metrics = {0, 0, 0};
    int currentTime = 0;  // Tracks the current time
    int completed = 0;    // Tracks the number of completed processes

    // Initialize remaining time for all processes
    for (int i = 0; i < n; i++) {
        proc[i].remainingTime = proc[i].burstTime;
    }

    // Create a queue to simulate the round-robin scheduling
    int *queue = (int*)malloc(n * sizeof(int)); 
    int front = 0, rear = 0;

    // Enqueue processes that have already arrived
    for (int i = 0; i < n; i++) {
        if (proc[i].arrivalTime <= currentTime) {
            queue[rear++] = i;
        }
    }

    // Round Robin Simulation
    while (completed < n) {
        int idx = queue[front++];  // Get the next process to execute
        front = front % n;  // Wrap around the front index if needed

        Process *p = &proc[idx];  // Get the current process

        // Set start time for the process when it is first executed
        if (p->remainingTime == p->burstTime) {
            p->startTime = currentTime;
        }

        // Calculate time spent in this round
        int timeSpent = (p->remainingTime <= timeQuantum) ? p->remainingTime : timeQuantum;
        p->remainingTime -= timeSpent;
        currentTime += timeSpent;

        // If the process has finished executing, calculate metrics
        if (p->remainingTime == 0) {
            p->completionTime = currentTime;
            completed++;

            // Calculate metrics for the process
            int turnaroundTime = p->completionTime - p->arrivalTime;
            int waitingTime = turnaroundTime - p->burstTime;
            int responseTime = p->startTime - p->arrivalTime;

            metrics.avgTurnaround += turnaroundTime;
            metrics.avgWaiting += waitingTime;
            metrics.avgResponse += responseTime;
        } else {
            // Enqueue the process again if it is not finished
            for (int i = 0; i < n; i++) {
                if (proc[i].arrivalTime <= currentTime && proc[i].remainingTime > 0) {
                    queue[rear++] = i;
                    break;
                }
            }
        }
    }

    // Free the allocated memory for the queue
    free(queue);

    // Calculate and return the average metrics
    return calculate_metrics(proc, n);
}
