#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

#include "memory_manager.h"
#include "process_queues.h"
#include "scheduler.h"
#include "pcb.h"

using namespace std;

// function to make copies of processes for testing different algorithms
// each algorithm needs its own copy so they dont interfere with each other
vector<PCB*> cloneProcesses(vector<PCB*>& original) {
    vector<PCB*> clones;
    for (int i = 0; i < original.size(); i++) {  // traditional for loop instead of auto
        PCB* p = original[i];
        PCB* newP = new PCB(*p);  // create new PCB copy
        newP->state = ProcessState::READY;
        newP->has_resource = false;
        clones.push_back(newP);
    }
    return clones;
}

int main() {
    srand(static_cast<unsigned>(time(nullptr)));


    // main memory going to be used throughout the program (for all schedulers)
    MemoryManager memory;

    // queues going to be used by all schedulers (blocked and ready)
    ProcessQueues queues;

    
    // created different schedulers for testing different algorithms
    Scheduler scheduler(&memory, &queues);
    Scheduler priorityScheduler(&memory, &queues);
    Scheduler sjfScheduler(&memory, &queues);
    Scheduler rrScheduler(&memory, &queues);



    // created list for processes. currently taking 3 resources only + 1 for no resource required

    vector<PCB*> allProcesses;
    int numProcesses = 20;
    vector<string> resources = {"file1", "printer", "disk", ""};

    for (int i = 1; i <= numProcesses; ++i) {
        PCB* p = new PCB();
        p->pid = i;
        p->priority = rand() % 10;
        p->state = ProcessState::NEW;
        p->has_resource = false;
        p->memory_required = 100 + rand() % 200; // by this I am able to only get around 10 processes for 1024 mb memory
        p->required_resource = resources[rand() % resources.size()];

        cout << "Creating Process with PID: " << p->pid
                  << ", Memory requirements: " << p->memory_required
                  << ", Resources required: " << p->required_resource << endl;

        if (memory.allocate(p->memory_required)) {
            scheduler.addToReadyQueue(p);
            allProcesses.push_back(p);
            cout << "Memory allocated in the SYSTEM successfully." << endl;
        } else {
            cout << "Not enough memory for current process, PID: " << p->pid << endl;
            delete p;
        }
    }


    // adding 3 more job scheduling algorithms. ie. Priority, sjf and round robin

    // vector<PCB*> cloned1 = cloneProcesses(allProcesses);
    // vector<PCB*> cloned2 = cloneProcesses(allProcesses);
    // vector<PCB*> cloned3 = cloneProcesses(allProcesses);


    // cout << endl << endl << endl;
    // cout << "-------------------------- FCFS Scheduling --------------------------" << endl;
    // cout << endl << endl << endl;

    // for (auto& p : cloned1) scheduler.addToReadyQueue(p);
    // scheduler.runFIFOScheduling();
    // scheduler.evaluatePerformance("FCFS Scheduling");


    

    cout << endl << endl << endl;
    cout << "-------------------------- Priority Scheduling --------------------------" << endl;
    cout << endl << endl << endl;


    
    for (int i = 0; i < allProcesses.size(); i++) {
        priorityScheduler.addToReadyQueue(allProcesses[i]);
    }
    priorityScheduler.runPriorityScheduling();
    priorityScheduler.evaluatePerformance("Priority Scheduling");



    
    cout << endl << endl << endl;
    cout << "-------------------------- SJF Scheduling --------------------------" << endl;
    cout << endl << endl << endl;




    for (int i = 0; i < allProcesses.size(); i++) {
        sjfScheduler.addToReadyQueue(allProcesses[i]);
    }
    sjfScheduler.runSJFScheduling();
    sjfScheduler.evaluatePerformance("SJF Scheduling");




    cout << endl << endl << endl;
    cout << "-------------------------- Round Robin Scheduling --------------------------" << endl;
    cout << endl << endl << endl;

    for (int i = 0; i < allProcesses.size(); i++) { 
        rrScheduler.addToReadyQueue(allProcesses[i]);
    }
    rrScheduler.runRoundRobinScheduling(4);
    rrScheduler.evaluatePerformance("Round Robin");



    cout << endl << endl << endl;
    cout << "-------------------------- Premptive Priority Scheduling --------------------------" << endl;
    cout << endl << endl << endl;



    // preemptive scheduling based on priority, usnig a single time slice need to change in future to 100s of iterations per cycle
    int cycles = 15;
    scheduler.simulateCPU(cycles);


    // freeing up space from allprocesses vector
    for (int i = 0; i < allProcesses.size(); i++) {
        PCB* p = allProcesses[i];
        if (p->state != ProcessState::TERMINATED) {
            memory.deallocate(p->memory_required);
        }
        delete p;
    }

    cout << endl << "------------------------- Simulation Complete ------------------------- " << endl;

    return 0;
}
