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

// comparator function for checking process priority
// this helps the priority queue know which process has higher priority
struct CompareProcessPriority {
    bool operator()(const PCB* a, const PCB* b) const {
        // Lower number means higher priority - so if a has bigger priority number
        // then b should come first, thats why we return true when a > b
        return a->priority > b->priority;
    }
};

class Scheduler {
private:
    // main ready queue that stores processes waiting to run
    std::priority_queue<PCB*, std::vector<PCB*>, CompareProcessPriority> readyQueue;

    std::queue<PCB*> fifoQueue; // queue for FCFS scheduling
    
    // map to store blocked processes for each resource
    std::unordered_map<std::string, std::queue<PCB*>> blockedQueues;
    
    // table to track which process owns which resource (resource name -> PID)
    std::unordered_map<std::string, int> resourceTable;
    
    PCB* runningProcess;           // pointer to currently running process
    MemoryManager* memoryManager;  // pointer to memory manager
    ProcessQueues* processQueues;  // pointer to process queues

    // variables for performance tracking - not fully implemented yet
    int total_turnaround_time;
    int total_waiting_time;
    int total_processes_terminated;
    int simulation_time;

public:
    // Constructor to initialize scheduler
    Scheduler(MemoryManager* mm, ProcessQueues* pq);
    
    // basic functions for process management
    void addProcess(PCB* p);
    void runSimulation();
    void evaluatePerformance(const std::string& algorithm);
    

    // QUEUE operations for process management - these are the main functions

    void addToReadyQueue(PCB* p);
    void scheduleNextProcess();
    void requestResource(PCB* p, const std::string& resourceName);
    void releaseResource(PCB* p, const std::string& resourceName);
    void terminateProcess(PCB* p);


    // CPU scheduling algorithms - different ways to schedule processes

    void simulateCPU(int cycles);
    void runPriorityScheduling();
    void runSJFScheduling();              // shortest job first
    void runRoundRobinScheduling(int timeQuantum);

private:
    // helper functions used internally
    void dispatch();
    void handleTermination(PCB* p);
};

#endif // SCHEDULER_H