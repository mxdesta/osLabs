#include "../include/ListAllocator.h"
#include <iostream>

namespace ListAllocator {

struct BlockHeader {
    size_t size;        // Размер блока
    bool is_free;       // Флаг занятости
    BlockHeader* next;  // Указатель на следующий блок
};

Allocator* createMemoryAllocator(void* realMemory, size_t memory_size) {
    Allocator* allocator = static_cast<Allocator*>(realMemory);
    allocator->memory = static_cast<char*>(realMemory) + sizeof(Allocator);
    allocator->memory_size = memory_size - sizeof(Allocator);
    allocator->used_memory = 0; // Инициализация used_memory

    // Инициализация первого блока
    BlockHeader* initial_block = static_cast<BlockHeader*>(allocator->memory);
    initial_block->size = allocator->memory_size;
    initial_block->is_free = true;
    initial_block->next = nullptr;

    return allocator;
}

void* alloc(Allocator* allocator, size_t block_size) {
    BlockHeader* current = static_cast<BlockHeader*>(allocator->memory);

    while (current) {
        if (current->is_free && current->size >= block_size) {
            // Если блок свободен и его размер достаточен
            if (current->size >= block_size + sizeof(BlockHeader) + 1) {
                // Разбиваем блок, если он слишком большой
                BlockHeader* next_block = reinterpret_cast<BlockHeader*>(
                    reinterpret_cast<char*>(current) + sizeof(BlockHeader) + block_size
                );
                next_block->size = current->size - block_size - sizeof(BlockHeader);
                next_block->is_free = true;
                next_block->next = current->next;

                // Обновляем текущий блок
                current->size = block_size;
                current->is_free = false;
                current->next = next_block;
            } else {
                // Если блок идеально подходит, просто выделяем его
                current->is_free = false;
            }

            // Обновляем used_memory только на размер выделенного блока
            allocator->used_memory += current->size;
            return reinterpret_cast<void*>(current + 1);  // Указатель на начало данных
        }
        current = current->next;
    }

    // Если не нашли подходящий блок
    return nullptr;
}

void freeBlock(Allocator* allocator, void* block) {
    if (!block) return;

    BlockHeader* header = reinterpret_cast<BlockHeader*>(block) - 1;
    header->is_free = true;

    // Обновляем used_memory только на размер освобожденного блока
    allocator->used_memory -= header->size;

    // Слияние соседних свободных блоков
    BlockHeader* current = static_cast<BlockHeader*>(allocator->memory);
    while (current) {
        if (current->is_free && current->next && current->next->is_free) {
            // Сливаем блоки
            current->size += sizeof(BlockHeader) + current->next->size;
            current->next = current->next->next;
        } else {
            current = current->next;
        }
    }
}

} // namespace ListAllocator