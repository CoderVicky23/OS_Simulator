#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <iostream>

// Memory manager class to handle process memory allocation and deallocation
// Simple implementation that tracks total and used memory
class MemoryManager {
private:
    int total_memory;  // total system memory available
    int used_memory;   // currently allocated memory

public:
    // Constructor - default total memory is 1024 MB
    MemoryManager(int total = 1024);

    // try to allocate memory for process, returns true if successful
    bool allocate(int mem_required);
    
    // free memory when process terminates
    void deallocate(int mem);
    
    // get how much memory is still available
    int get_available_memory() const;
    
    // print current memory usage for debugging
    void print_memory_status() const;
};

#endif // MEMORY_MANAGER_H