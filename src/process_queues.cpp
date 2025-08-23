#include "process_queues.h"
#include <iostream>

void ProcessQueues::add_to_ready(PCB* process) {
    ready_queue.push(process);
}

void ProcessQueues::add_to_blocked(const std::string& resource, PCB* process) {
    blocked_queue[resource].push_back(process);
}

PCB* ProcessQueues::pop_ready() {
    if (!ready_queue.empty()) {
        PCB* process = ready_queue.front();
        ready_queue.pop();
        return process;
    }
    return nullptr;
}

void ProcessQueues::release_resource(const std::string& resource) {
    auto it = blocked_queue.find(resource);
    if (it != blocked_queue.end()) {
        for (const auto& proc : it->second) {
            std::cout << "[Resource] Process " << proc->pid << " moved from Blocked to Ready (resource: " << resource << ")\n";
            ready_queue.push(proc);
        }
        blocked_queue.erase(it);
    }
}

void ProcessQueues::print_queues_status() const {
    std::cout << "[Ready Queue] Size: " << ready_queue.size() << "\n";
    std::cout << "[Blocked Queues]" << "\n";
    for (const auto& [res, list] : blocked_queue) {
        std::cout << "  Resource: " << res << " → Processes: ";
        for (const auto& proc : list) {
            std::cout << proc->pid << " ";
        }
        std::cout << "\n";
    }
}