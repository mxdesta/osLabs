#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <cstddef>

// Базовая структура аллокатора
struct Allocator {
    void* memory;       // Указатель на начало памяти
    size_t memory_size; // Размер доступной памяти
    size_t used_memory; // Используемая память
};

// Интерфейс аллокатора
Allocator* createMemoryAllocator(void* realMemory, size_t memory_size);
void* alloc(Allocator* allocator, size_t block_size);
void freeBlock(Allocator* allocator, void* block);

#endif // ALLOCATOR_H