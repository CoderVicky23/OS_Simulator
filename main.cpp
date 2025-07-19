#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "memory_manager.h"
#include "process_queues.h"
#include "scheduler.h"
#include "pcb.h"

std::vector<PCB*> cloneProcesses(const std::vector<PCB*>& original) {
    std::vector<PCB*> clones;
    for (const auto& p : original) {
        PCB* newP = new PCB(*p);
        newP->state = ProcessState::READY;
        newP->has_resource = false;
        clones.push_back(newP);
    }
    return clones;
}

int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // Initialize system components
    MemoryManager memory;
    ProcessQueues queues;
    Scheduler scheduler(&memory, &queues);
    Scheduler priorityScheduler(&memory, &queues);
    Scheduler sjfScheduler(&memory, &queues);
    Scheduler rrScheduler(&memory, &queues);

    // Create sample processes
    std::vector<PCB*> allProcesses;
    int numProcesses = 20;
    std::vector<std::string> resources = {"file1", "printer", "disk"};

    for (int i = 1; i <= numProcesses; ++i) {
        PCB* p = new PCB();
        p->pid = i;
        p->priority = rand() % 10;
        p->state = ProcessState::NEW;
        p->has_resource = false;
        p->memory_required = 100 + rand() % 200;
        p->required_resource = resources[rand() % resources.size()];

        std::cout << "[Create] Process PID: " << p->pid
                  << ", Memory: " << p->memory_required
                  << ", Resource: " << p->required_resource << "\n";

        if (memory.allocate(p->memory_required)) {
            scheduler.addToReadyQueue(p);
            allProcesses.push_back(p);
        } else {
            std::cout << "[Rejected] Not enough memory for PID: " << p->pid << "\n";
            delete p;
        }
    }

    std::vector<PCB*> cloned1 = cloneProcesses(allProcesses);
    std::vector<PCB*> cloned2 = cloneProcesses(allProcesses);
    std::vector<PCB*> cloned3 = cloneProcesses(allProcesses);

    for (auto& p : cloned1) priorityScheduler.addToReadyQueue(p);
    priorityScheduler.runPriorityScheduling();
    priorityScheduler.evaluatePerformance("Priority Scheduling");

    for (auto& p : cloned2) sjfScheduler.addToReadyQueue(p);
    sjfScheduler.runSJFScheduling();
    sjfScheduler.evaluatePerformance("SJF Scheduling");

    for (auto& p : cloned3) rrScheduler.addToReadyQueue(p);
    rrScheduler.runRoundRobinScheduling(4);
    rrScheduler.evaluatePerformance("Round Robin");

    // Print initial memory status
    memory.print_memory_status();

    // Simulate CPU execution cycles
    int cycles = 15;
    scheduler.simulateCPU(cycles);
    // priorityScheduler.runPriorityScheduling();
    // priorityScheduler.evaluatePerformance("Priority Scheduling");

    // sjfScheduler.runSJFScheduling();
    // sjfScheduler.evaluatePerformance("SJF Scheduling");

    // rrScheduler.runRoundRobinScheduling(4); // 4 ms time slice
    // rrScheduler.evaluatePerformance("Round Robin");

    // Clean up
    for (PCB* p : allProcesses) {
        if (p->state != ProcessState::TERMINATED) {
            memory.deallocate(p->memory_required);
        }
        delete p;
    }

    std::cout << "\n[Simulation Complete] Final memory status:\n";
    memory.print_memory_status();

    return 0;
}
