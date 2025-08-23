#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

#include "memory_manager.h"
#include "process_queues.h"
#include "scheduler.h"
#include "pcb.h"

using namespace std;

vector<PCB*> cloneProcesses(vector<PCB*>& original) {
    vector<PCB*> clones;
    for (const auto& p : original) {
        PCB* newP = new PCB(*p);
        newP->state = ProcessState::READY;
        newP->has_resource = false;
        clones.push_back(newP);
    }
    return clones;
}

int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    // Initialize system components
    MemoryManager memory;
    ProcessQueues queues;
    Scheduler scheduler(&memory, &queues);
    Scheduler priorityScheduler(&memory, &queues);
    Scheduler sjfScheduler(&memory, &queues);
    Scheduler rrScheduler(&memory, &queues);

    // Create sample processes
    vector<PCB*> allProcesses;
    int numProcesses = 20;
    vector<string> resources = {"file1", "printer", "disk", ""};

    for (int i = 1; i <= numProcesses; ++i) {
        PCB* p = new PCB();
        p->pid = i;
        p->priority = rand() % 10;
        p->state = ProcessState::NEW;
        p->has_resource = false;
        p->memory_required = 100 + rand() % 200;
        p->required_resource = resources[rand() % resources.size()];

        cout << "[Create] Process PID: " << p->pid
                  << ", Memory: " << p->memory_required
                  << ", Resource: " << p->required_resource << "\n";

        if (memory.allocate(p->memory_required)) {
            scheduler.addToReadyQueue(p);
            allProcesses.push_back(p);
        } else {
            cout << "[Rejected] Not enough memory for PID: " << p->pid << "\n";
            delete p;
        }
    }

    vector<PCB*> cloned2 = cloneProcesses(allProcesses);
    vector<PCB*> cloned3 = cloneProcesses(allProcesses);
    vector<PCB*> cloned1 = cloneProcesses(allProcesses);

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

    // Clean up
    for (PCB* p : allProcesses) {
        if (p->state != ProcessState::TERMINATED) {
            memory.deallocate(p->memory_required);
        }
        delete p;
    }

    cout << "\n[Simulation Complete] Final memory status:\n";
    memory.print_memory_status();

    return 0;
}
