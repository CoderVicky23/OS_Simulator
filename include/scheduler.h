#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <queue>
#include <unordered_map>
#include <string>
#include <iostream>
#include <cstdlib>
#include "pcb.h"
#include "memory_manager.h"
#include "process_queues.h"

class Scheduler {
private:
    std::queue<PCB*> readyQueue;
    std::unordered_map<std::string, std::queue<PCB*>> blockedQueues;
    std::unordered_map<std::string, int> resourceTable; // resource → PID
    PCB* runningProcess;
    MemoryManager* memoryManager;
    ProcessQueues* processQueues;

public:
    Scheduler(MemoryManager* mm, ProcessQueues* pq);

    void scheduleNextProcess();
    void requestResource(PCB* p, const std::string& resourceName);
    void releaseResource(PCB* p, const std::string& resourceName);
    void terminateProcess(PCB* p);
    void simulateCPU(int cycles);
    void addToReadyQueue(PCB* p);

    void runPriorityScheduling();
    void runSJFScheduling();
    void runRoundRobinScheduling(int timeQuantum);
    void evaluatePerformance(const std::string& algorithm);
};

#endif // SCHEDULER_H