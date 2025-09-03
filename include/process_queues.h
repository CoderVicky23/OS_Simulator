#ifndef PROCESS_QUEUES_H
#define PROCESS_QUEUES_H

#include <queue>
#include <unordered_map>
#include <list>
#include <vector>
#include "pcb.h"

// class to manage different process queues in the system
// handles ready queue and blocked queues for resources
class ProcessQueues {
private:
    std::queue<PCB*> ready_queue;     // processes ready to run
    std::unordered_map<std::string, std::list<PCB*>> blocked_queue;  // processes blocked on resources

public:
    // add process to ready queue
    void add_to_ready(PCB* process);
    
    // add process to blocked queue for specific resource
    void add_to_blocked(const std::string& resource, PCB* process);
    
    // get next process from ready queue
    PCB* pop_ready();
    
    // release a resource and move blocked processes to ready
    void release_resource(const std::string& resource);
    
    // print status of all queues
    void print_queues_status() const;
};

#endif // PROCESS_QUEUES_H