#include "memory_manager.h"

MemoryManager::MemoryManager(int total) : total_memory(total), used_memory(0) {}

bool MemoryManager::allocate(int mem_required) {
    if (used_memory + mem_required <= total_memory) {
        used_memory += mem_required;
        return true;
    }
    return false;
}

void MemoryManager::deallocate(int mem) {
    used_memory -= mem;
    if (used_memory < 0) used_memory = 0;
}

int MemoryManager::get_available_memory() const {
    return total_memory - used_memory;
}

void MemoryManager::print_memory_status() const {
    std::cout << "[Memory Status] Used: " << used_memory << " MB / Total: " << total_memory << " MB\n";
}