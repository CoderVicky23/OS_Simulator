#include "scheduler.h"

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
        runningProcess = readyQueue.front();
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