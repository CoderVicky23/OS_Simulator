#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <iostream>

class MemoryManager {
private:
    int total_memory;
    int used_memory;

public:
    MemoryManager(int total = 1024);

    bool allocate(int mem_required);
    void deallocate(int mem);
    int get_available_memory() const;
    void print_memory_status() const;
};

#endif // MEMORY_MANAGER_H