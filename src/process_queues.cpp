#include "process_queues.h"
#include <iostream>

using namespace std;

// add a process to the ready queue
void ProcessQueues::add_to_ready(PCB* process) {
    ready_queue.push(process);
}

// add a process to blocked queue for a specific resource
void ProcessQueues::add_to_blocked(const string& resource, PCB* process) {
    blocked_queue[resource].push_back(process);
}

// get the next process from ready queue
PCB* ProcessQueues::pop_ready() {
    if (!ready_queue.empty()) {
        PCB* process = ready_queue.front();
        ready_queue.pop();
        return process;
    }
    return nullptr;  // no process available
}

// when a resource becomes free, move all blocked processes to ready queue
void ProcessQueues::release_resource(const string& resource) {
    auto it = blocked_queue.find(resource);
    if (it != blocked_queue.end()) {
        // move all processes waiting for this resource to ready queue
        for (auto proc_it = it->second.begin(); proc_it != it->second.end(); proc_it++) {
            PCB* proc = *proc_it;
            cout << "[Resource] Process " << proc->pid << " moved from Blocked to Ready (resource: " << resource << ")" << endl;
            ready_queue.push(proc);
        }
        blocked_queue.erase(it);  // remove the resource entry
    }
}

// print status of all queues for debugging
void ProcessQueues::print_queues_status() const {
    cout << "[Ready Queue] Size: " << ready_queue.size() << endl;
    cout << "[Blocked Queues]" << endl;
    
    // iterate through blocked queues manually
    for (auto it = blocked_queue.begin(); it != blocked_queue.end(); it++) {
        cout << "  Resource: " << it->first << " -> Processes: ";
        for (auto proc_it = it->second.begin(); proc_it != it->second.end(); proc_it++) {
            cout << (*proc_it)->pid << " ";
        }
        cout << endl;
    }
}