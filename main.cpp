// ============================ main.cpp ============================
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "memory_manager.h"
#include "process_queues.h"
#include "scheduler.h"
#include "pcb.h"

int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // Initialize system components
    MemoryManager memory;
    ProcessQueues queues;
    Scheduler scheduler(&memory, &queues);

    // Create sample processes
    std::vector<PCB*> allProcesses;
    int numProcesses = 5;
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

    // Print initial memory status
    memory.print_memory_status();

    // Simulate CPU execution cycles
    int cycles = 15;
    scheduler.simulateCPU(cycles);

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
