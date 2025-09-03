#include "memory_manager.h"

using namespace std;

// Constructor - create memory manager with specified total memory
// default is 1024 MB if no value provided
MemoryManager::MemoryManager(int total) {
    total_memory = total;
    used_memory = 0;  // start with no memory used
}

// try to allocate memory for a process
// returns true if successful, false if not enough memory
bool MemoryManager::allocate(int mem_required) {
    if (used_memory + mem_required <= total_memory) {
        used_memory = used_memory + mem_required;  // allocate the memory
        return true;  // success
    }
    return false;  // not enough memory available
}

// free up memory when process terminates
void MemoryManager::deallocate(int mem) {
    used_memory = used_memory - mem;
    if (used_memory < 0) {
        used_memory = 0;  // make sure we dont go negative
    }
}

// calculate how much memory is still available
int MemoryManager::get_available_memory() const {
    return total_memory - used_memory;
}

// print current memory usage status
void MemoryManager::print_memory_status() const {
    cout << "[Memory Status] Used: " << used_memory << " MB / Total: " << total_memory << " MB" << endl;
}