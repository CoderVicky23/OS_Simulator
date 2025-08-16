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
    int memory_required; // Can be used as burst time for SJF

    // For performance metrics
    int arrival_time;
    int burst_time; // A more explicit field for CPU time needed
    int remaining_time;
    int completion_time;
    int turnaround_time;
    int waiting_time;
};

#endif // PCB_H