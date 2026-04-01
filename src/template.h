#pragma once

#include <cstddef>
#include <cstdint>
#include <algorithm>

namespace sjtu {

constexpr size_t MEMORY_SIZE = 1024 * 1024; // 1 MB

struct MemoryBlock{
    int address;
    int size;
    bool is_free;
    MemoryBlock* next = nullptr;
};

inline MemoryBlock* head = nullptr;

inline MemoryBlock* splitBlock(MemoryBlock* block, size_t size) {
    if (block->size < static_cast<int>(size + sizeof(MemoryBlock))) {
        return nullptr;
    }
    MemoryBlock* new_block = new MemoryBlock;
    new_block->address = block->address + sizeof(MemoryBlock) + static_cast<int>(size);
    new_block->size = block->size - static_cast<int>(size) - static_cast<int>(sizeof(MemoryBlock));
    new_block->is_free = true;
    new_block->next = block->next;
    block->size = static_cast<int>(size);
    block->next = new_block;
    return new_block;
}

inline void mergeBlock(MemoryBlock* block){
    MemoryBlock* next = block->next;
    if (next == nullptr) return;
    block->size += static_cast<int>(sizeof(MemoryBlock)) + next->size;
    block->next = next->next;
    delete next;
}

inline MemoryBlock* allocate(int size){
    if (size <= 0) return nullptr;
    MemoryBlock* cur = head;
    while (cur) {
        if (cur->is_free && cur->size >= size) {
            if (cur->size >= size + static_cast<int>(sizeof(MemoryBlock)) + 1) {
                splitBlock(cur, static_cast<size_t>(size));
            }
            cur->is_free = false;
            return cur;
        }
        cur = cur->next;
    }
    return nullptr;
}

inline void deallocate(MemoryBlock* ptr){
    if (!ptr) return;
    ptr->is_free = true;
    if (ptr->next && ptr->next->is_free) {
        mergeBlock(ptr);
    }
}

inline void resetMemory(){
    MemoryBlock* temp = head;
    while (temp) {
        MemoryBlock* next = temp->next;
        delete temp;
        temp = next;
    }
    head = new MemoryBlock;
    head->address = 0;
    head->size = static_cast<int>(MEMORY_SIZE - sizeof(MemoryBlock));
    head->is_free = true;
    head->next = nullptr;
}

} // namespace sjtu

