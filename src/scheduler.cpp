#include "scheduler.h"
#include <algorithm>
#include <iomanip>

using namespace std;

// initialize the scheduler with memory manager and process queues
// passing pointers to avoid copying big objects
Scheduler::Scheduler(MemoryManager* mm, ProcessQueues* pq) {
    // Store the pointers for later use
    memoryManager = mm;
    processQueues = pq;
    runningProcess = nullptr;  // no process running initially - shows the initial state of cpu 
    
    // performance calculating variables
    total_turnaround_time = 0;
    total_waiting_time = 0;
    total_processes_terminated = 0;
    simulation_time = 0;
}



// Function to add processes to ready queue
void Scheduler::addToReadyQueue(PCB* p) {
    p->state = ProcessState::READY;  // mark process as ready
    readyQueue.push(p);
    cout << "[Ready] Process " << p->pid << " added to Ready Queue" << endl;
}



// this function picks the next process to run
void Scheduler::scheduleNextProcess() {
    // if there's already a process running, put it back in queue
    if (runningProcess != nullptr) {
        cout << "[Context Switch] Process " << runningProcess->pid << " put back to Ready Queue" << endl;
        addToReadyQueue(runningProcess);
    }

    // check if we have any processes waiting to run
    if (!readyQueue.empty()) {
        // get the process with highest priority (lowest number = higher priority)
        runningProcess = readyQueue.top();
        readyQueue.pop();
        runningProcess->state = ProcessState::RUNNING;
        cout << "[Scheduler] Running PID: " << runningProcess->pid << endl;
    } else {
        // no processes to run, CPU is idle
        runningProcess = nullptr;
        cout << "[Scheduler] No process to schedule. CPU Idle." << endl;
    }
}

// function to request a resource for a process
// if resource is free, give it to process, otherwise block the process
void Scheduler::requestResource(PCB* p, const string& resourceName) {
    if (resourceTable[resourceName] == 0) {
        // resource is free, allocate it
        resourceTable[resourceName] = p->pid;
        p->has_resource = true;
        cout << "[Resource] Allocated " << resourceName << " to PID: " << p->pid << endl;
    } else {
        // resource is busy, block the process
        cout << "[Resource] PID: " << p->pid << " blocked on " << resourceName << endl;
        p->state = ProcessState::BLOCKED;
        blockedQueues[resourceName].push(p);
        runningProcess = nullptr;  // process is not running anymore
    }
}

// when a process releases a resource
// check if any other process was waiting for it
void Scheduler::releaseResource(PCB* p, const string& resourceName) {
    if (resourceTable[resourceName] != p->pid) return;  // this process doesn't own this resource

    cout << "[Resource] PID: " << p->pid << " released " << resourceName << endl;
    resourceTable[resourceName] = 0;  // mark resource as free

    // check if any process was waiting for this resource
    if (!blockedQueues[resourceName].empty()) {
        PCB* nextProc = blockedQueues[resourceName].front();
        blockedQueues[resourceName].pop();
        nextProc->state = ProcessState::READY;
        nextProc->has_resource = true;
        resourceTable[resourceName] = nextProc->pid;
        addToReadyQueue(nextProc);
        cout << "[Unblock] PID: " << nextProc->pid << " unblocked from " << resourceName << endl;
    }
}

// function to terminate a process
// clean up memory and resources before killing it
void Scheduler::terminateProcess(PCB* p) {
    cout << "[Terminate] PID: " << p->pid << " terminated." << endl;
    p->state = ProcessState::TERMINATED;
    memoryManager->deallocate(p->memory_required);
    releaseResource(p, p->required_resource);
    runningProcess = nullptr;  // CPU becomes free
}

// main CPU simulation loop
// this runs for specified number of cycles

void Scheduler::simulateCPU(int cycles) { // performs the preemptive priority scheduling


    for (int i = 0; i < cycles; i++) {  // using traditional for loop style
        cout << endl << "[Cycle] " << i + 1 << "/" << cycles << endl;

        // if no process is running or current process stopped, schedule next one
        if (runningProcess == nullptr || runningProcess->state != ProcessState::RUNNING) {
            scheduleNextProcess();
        }

        // if we have a running process, do some work
        if (runningProcess != nullptr) {
            // check if process needs resource but doesnt have it
            if (!runningProcess->has_resource && !runningProcess->required_resource.empty()) {
                requestResource(runningProcess, runningProcess->required_resource);
                if (runningProcess == nullptr) continue;  // process got blocked
            }

            // simulate some random action - terminate, release resource, or continue
            int action = rand() % 3;
            if (action == 0) {
                terminateProcess(runningProcess);
            } else if (action == 1) {
                releaseResource(runningProcess, runningProcess->required_resource);
                scheduleNextProcess();
            } else {
                cout << "[Execute] PID: " << runningProcess->pid << " continues execution." << endl;
            }
        }
    }
}

// Priority scheduling algorithm
// runs processes based on their priority value (lower number = higher priority)
void Scheduler::runPriorityScheduling() {
    vector<PCB*> processes;
    // make a copy of the queue so we don't mess up the original
    priority_queue<PCB*, vector<PCB*>, CompareProcessPriority> tempQueue = readyQueue;
    while (!readyQueue.empty()) {
        processes.push_back(readyQueue.top());
        readyQueue.pop();
    }
    
    cout << endl << "[Priority Scheduling]" << endl;
    int i;
    for (i = 0; i < processes.size(); i++) {  // traditional for loop
        PCB* p = processes[i];
        cout << "[Run] PID: " << p->pid << " Priority: " << p->priority << endl;
    }
}

// Shortest Job First scheduling
// runs the process with smallest memory requirement first
void Scheduler::runSJFScheduling() {
    vector<PCB*> processes;
    // copy all processes from priority queue
    priority_queue<PCB*, vector<PCB*>, CompareProcessPriority> tempQueue = readyQueue;
    while (!tempQueue.empty()) {
        processes.push_back(tempQueue.top());
        tempQueue.pop();
    }
    
    // sort by memory requirement (using it as job size) using standard sort function
    sort(processes.begin(), processes.end(), [](PCB* a, PCB* b) {
        return a->memory_required < b->memory_required;
    });

    cout << endl << "[Shortest Job First Scheduling]" << endl;
    for (int i = 0; i < processes.size(); i++) {
        PCB* p = processes[i];
        cout << "[Run] PID: " << p->pid << " Memory Req: " << p->memory_required << endl;
    }
}

// Round Robin scheduling - give each process equal time
// time quantum determines how long each process gets to run
void Scheduler::runRoundRobinScheduling(int timeQuantum) {
    vector<PCB*> processes;
    // copy processes from priority queue to a regular vector
    priority_queue<PCB*, vector<PCB*>, CompareProcessPriority> tempQueue = readyQueue;

    while (!tempQueue.empty()) {
        processes.push_back(tempQueue.top());
        tempQueue.pop();
    }
    
    // create a simple queue for round robin
    queue<PCB*> rrQueue;
    for (int i = 0; i < processes.size(); i++) {
        rrQueue.push(processes[i]);
    }
    
    cout << endl << "[Round Robin Scheduling] (Time Quantum: " << timeQuantum << ")" << endl;
    int cycle = 0;
    while (!rrQueue.empty()) {
        PCB* current = rrQueue.front();
        rrQueue.pop();
        cycle = cycle + 1;  // simple increment
        cout << "[Cycle " << cycle << "] Running PID: " << current->pid << endl;
        rrQueue.push(current);  // put back at end of queue
        if (cycle > processes.size() * 2) {
            break;  // stop infinite loop for demo purposes
        }
    }
}

// Performance evaluation function
// TODO: need to implement proper metrics calculation
void Scheduler::evaluatePerformance(const string& algorithm) {
    cout << endl << "[Evaluation: " << algorithm << "]" << endl;
    cout << "(Metrics simulation not fully implemented yet)" << endl;
    cout << "Turnaround Time: Will calculate later" << endl;
    cout << "Waiting Time: Will calculate later" << endl;
    cout << "CPU Utilization: Will calculate later" << endl;
}

// Simple wrapper function to add process
void Scheduler::addProcess(PCB* p) {
    addToReadyQueue(p);
}

// Run the simulation with default settings
void Scheduler::runSimulation() {
    simulateCPU(10);  // run 10 cycles by default
}

// Helper function for dispatching
void Scheduler::dispatch() {
    scheduleNextProcess();
}

// Helper function for termination
void Scheduler::handleTermination(PCB* p) {
    terminateProcess(p);
}