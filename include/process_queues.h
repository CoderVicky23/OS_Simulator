#ifndef PROCESS_QUEUES_H
#define PROCESS_QUEUES_H

#include <queue>
#include <unordered_map>
#include <list>
#include <vector>
#include "pcb.h"

class ProcessQueues {
private:
    std::queue<PCB*> ready_queue;
    std::unordered_map<std::string, std::list<PCB*>> blocked_queue;

public:
    void add_to_ready(PCB* process);
    void add_to_blocked(const std::string& resource, PCB* process);
    PCB* pop_ready();
    void release_resource(const std::string& resource);
    void print_queues_status() const;
};

#endif // PROCESS_QUEUES_H