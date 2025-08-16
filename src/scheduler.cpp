#include "scheduler.h"

#include <algorithm>
#include <iomanip>

Scheduler::Scheduler(MemoryManager* mm, ProcessQueues* pq)
    : memoryManager(mm), processQueues(pq), runningProcess(nullptr) {}

void Scheduler::addToReadyQueue(PCB* p) {
    p->state = ProcessState::READY;
    readyQueue.push(p);
    std::cout << "[Ready] Process " << p->pid << " added to Ready Queue\n";
}

void Scheduler::scheduleNextProcess() {
    if (runningProcess != nullptr) {
        std::cout << "[Context Switch] Process " << runningProcess->pid << " put back to Ready Queue\n";
        addToReadyQueue(runningProcess);
    }

    if (!readyQueue.empty()) {
        runningProcess = readyQueue.top();
        readyQueue.pop();
        runningProcess->state = ProcessState::RUNNING;
        std::cout << "[Scheduler] Running PID: " << runningProcess->pid << "\n";
    } else {
        runningProcess = nullptr;
        std::cout << "[Scheduler] No process to schedule. CPU Idle.\n";
    }
}

void Scheduler::requestResource(PCB* p, const std::string& resourceName) {
    if (resourceTable[resourceName] == 0) {
        resourceTable[resourceName] = p->pid;
        p->has_resource = true;
        std::cout << "[Resource] Allocated " << resourceName << " to PID: " << p->pid << "\n";
    } else {
        std::cout << "[Resource] PID: " << p->pid << " blocked on " << resourceName << "\n";
        p->state = ProcessState::BLOCKED;
        blockedQueues[resourceName].push(p);
        runningProcess = nullptr;
    }
}

void Scheduler::releaseResource(PCB* p, const std::string& resourceName) {
    if (resourceTable[resourceName] != p->pid) return;

    std::cout << "[Resource] PID: " << p->pid << " released " << resourceName << "\n";
    resourceTable[resourceName] = 0;

    if (!blockedQueues[resourceName].empty()) {
        PCB* nextProc = blockedQueues[resourceName].front();
        blockedQueues[resourceName].pop();
        nextProc->state = ProcessState::READY;
        nextProc->has_resource = true;
        resourceTable[resourceName] = nextProc->pid;
        addToReadyQueue(nextProc);
        std::cout << "[Unblock] PID: " << nextProc->pid << " unblocked from " << resourceName << "\n";
    }
}

void Scheduler::terminateProcess(PCB* p) {
    std::cout << "[Terminate] PID: " << p->pid << " terminated.\n";
    p->state = ProcessState::TERMINATED;
    memoryManager->deallocate(p->memory_required);
    releaseResource(p, p->required_resource);
    runningProcess = nullptr;
}

void Scheduler::simulateCPU(int cycles) {
    for (int i = 0; i < cycles; ++i) {
        std::cout << "\n[Cycle] " << i + 1 << "/" << cycles << "\n";

        if (runningProcess == nullptr || runningProcess->state != ProcessState::RUNNING) {
            scheduleNextProcess();
        }

        if (runningProcess != nullptr) {
            if (!runningProcess->has_resource && !runningProcess->required_resource.empty()) {
                requestResource(runningProcess, runningProcess->required_resource);
                if (runningProcess == nullptr) continue;
            }

            // Simulate work or randomly terminate
            int action = rand() % 3;
            if (action == 0) {
                terminateProcess(runningProcess);
            } else if (action == 1) {
                releaseResource(runningProcess, runningProcess->required_resource);
                scheduleNextProcess();
            } else {
                std::cout << "[Execute] PID: " << runningProcess->pid << " continues execution.\n";
            }
        }
    }
}

void Scheduler::runPriorityScheduling() {
    std::vector<PCB*> processes;
    // Create a temporary copy of the priority queue
    std::priority_queue<PCB*, std::vector<PCB*>, CompareProcessPriority> tempQueue = readyQueue;
    while (!tempQueue.empty()) {
        processes.push_back(tempQueue.top());
        tempQueue.pop();
    }
    
    std::cout << "\n[Priority Scheduling]\n";
    for (auto& p : processes) {
        std::cout << "[Run] PID: " << p->pid << " Priority: " << p->priority << "\n";
    }
}

void Scheduler::runSJFScheduling() {
    std::vector<PCB*> processes;
    // Create a temporary copy of the priority queue
    std::priority_queue<PCB*, std::vector<PCB*>, CompareProcessPriority> tempQueue = readyQueue;
    while (!tempQueue.empty()) {
        processes.push_back(tempQueue.top());
        tempQueue.pop();
    }
    std::sort(processes.begin(), processes.end(), [](PCB* a, PCB* b) {
        return a->memory_required < b->memory_required;
    });

    std::cout << "\n[Shortest Job First Scheduling]\n";
    for (auto& p : processes) {
        std::cout << "[Run] PID: " << p->pid << " Memory Req: " << p->memory_required << "\n";
    }
}

void Scheduler::runRoundRobinScheduling(int timeQuantum) {
    std::vector<PCB*> processes;
    // Create a temporary copy of the priority queue
    std::priority_queue<PCB*, std::vector<PCB*>, CompareProcessPriority> tempQueue = readyQueue;
    while (!tempQueue.empty()) {
        processes.push_back(tempQueue.top());
        tempQueue.pop();
    }
    
    std::queue<PCB*> rrQueue;
    for (auto& p : processes) {
        rrQueue.push(p);
    }
    
    std::cout << "\n[Round Robin Scheduling] (Time Quantum: " << timeQuantum << ")\n";
    int cycle = 0;
    while (!rrQueue.empty()) {
        PCB* current = rrQueue.front();
        rrQueue.pop();
        std::cout << "[Cycle " << ++cycle << "] Running PID: " << current->pid << "\n";
        rrQueue.push(current);
        if (cycle > processes.size() * 2) break; // avoid infinite loop in demo
    }
}

void Scheduler::evaluatePerformance(const std::string& algorithm) {
    std::cout << "\n[Evaluation: " << algorithm << "]\n";
    std::cout << "(Metrics simulation not fully implemented)\n";
    std::cout << "Turnaround Time: Simulated\n";
    std::cout << "Waiting Time: Simulated\n";
    std::cout << "CPU Utilization: Simulated\n";
}

void Scheduler::addProcess(PCB* p) {
    addToReadyQueue(p);
}

void Scheduler::runSimulation() {
    simulateCPU(10); // Default simulation cycles
}

void Scheduler::dispatch() {
    scheduleNextProcess();
}

void Scheduler::handleTermination(PCB* p) {
    terminateProcess(p);
}