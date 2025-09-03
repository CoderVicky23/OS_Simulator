#ifndef PCB_H
#define PCB_H

#include <string>

// different states a process can be in during its lifetime
enum class ProcessState { NEW, READY, RUNNING, BLOCKED, TERMINATED };

// Process Control Block - stores all information about a process
struct PCB {
    int pid;                        // process ID
    ProcessState state;             // current state of process
    int priority;                   // priority level (lower number = higher priority)
    std::string required_resource;  // resource this process needs
    bool has_resource;              // does process currently own the resource?
    int memory_required;            // how much memory this process needs

    // For performance metrics calculation (not fully implemented yet)
    int arrival_time;      // when process arrived
    int burst_time;        // how long process needs CPU
    int remaining_time;    // time left for process
    int completion_time;   // when process finished
    int turnaround_time;   // total time from arrival to completion
    int waiting_time;      // time spent waiting
};

#endif // PCB_H