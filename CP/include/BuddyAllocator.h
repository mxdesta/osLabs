#ifndef BUDDYALLOCATOR_H
#define BUDDYALLOCATOR_H

#include "Allocator.h"

namespace BuddyAllocator {

    // Объявляем структуру BlockHeader
    struct BlockHeader {
        size_t size;        // Размер блока
        bool is_free;       // Флаг занятости
        BlockHeader* next;  // Указатель на следующий блок
    };

    // Наследуемся от основной структуры Allocator
    struct Allocator : public ::Allocator {
        BlockHeader* free_lists[32]; // Списки свободных блоков для каждого размера (2^5 ... 2^31)
    };

    Allocator* createMemoryAllocator(void* realMemory, size_t memory_size);
    void* alloc(Allocator* allocator, size_t block_size);
    void freeBlock(Allocator* allocator, void* block);

} 

#endif // BUDDYALLOCATOR_H