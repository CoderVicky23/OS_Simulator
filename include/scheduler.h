#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <queue>
#include <unordered_map>
#include <string>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include "pcb.h"
#include "memory_manager.h"
#include "process_queues.h"

// Custom comparator for the priority queue
struct CompareProcessPriority {
    bool operator()(const PCB* a, const PCB* b) const {
        // Lower number means higher priority
        return a->priority > b->priority;
    }
};

class Scheduler {
private:
    // Use a structure that fits the algorithm. PriorityQueue is great for Priority Scheduling.
    std::priority_queue<PCB*, std::vector<PCB*>, CompareProcessPriority> readyQueue;
    std::unordered_map<std::string, std::queue<PCB*>> blockedQueues;
    std::unordered_map<std::string, int> resourceTable; // resource → PID
    PCB* runningProcess;
    MemoryManager* memoryManager;
    ProcessQueues* processQueues;

    // Performance metrics
    int total_turnaround_time = 0;
    int total_waiting_time = 0;
    int total_processes_terminated = 0;
    int simulation_time = 0;

public:
    Scheduler(MemoryManager* mm, ProcessQueues* pq);
    void addProcess(PCB* p);
    void runSimulation();
    void evaluatePerformance(const std::string& algorithm);
    
    // Additional methods used in implementation
    void addToReadyQueue(PCB* p);
    void scheduleNextProcess();
    void requestResource(PCB* p, const std::string& resourceName);
    void releaseResource(PCB* p, const std::string& resourceName);
    void terminateProcess(PCB* p);
    void simulateCPU(int cycles);
    void runPriorityScheduling();
    void runSJFScheduling();
    void runRoundRobinScheduling(int timeQuantum);

private:
    void dispatch();
    void handleTermination(PCB* p);
};

#endif // SCHEDULER_H