#ifndef LISTALLOCATOR_H
#define LISTALLOCATOR_H

#include "Allocator.h"

namespace ListAllocator {

    // Наследуемся от основной структуры Allocator
    struct Allocator : public ::Allocator {};

    Allocator* createMemoryAllocator(void* realMemory, size_t memory_size);
    void* alloc(Allocator* allocator, size_t block_size);
    void freeBlock(Allocator* allocator, void* block);

} // namespace ListAllocator

#endif // LISTALLOCATOR_H