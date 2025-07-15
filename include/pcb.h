#ifndef PCB_H
#define PCB_H

#include <string>

enum class ProcessState { NEW, READY, RUNNING, BLOCKED, TERMINATED };

struct PCB {
    int pid;
    ProcessState state;
    int priority;
    std::string required_resource;
    bool has_resource;
    int memory_required;
};

#endif // PCB_H