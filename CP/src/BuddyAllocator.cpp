#include "../include/BuddyAllocator.h"
#include <iostream>
#include <cmath>

namespace BuddyAllocator {

Allocator* createMemoryAllocator(void* realMemory, size_t memory_size) {
    // Приводим realMemory к типу Allocator*
    Allocator* allocator = static_cast<Allocator*>(realMemory);

    // Вычисляем начало области памяти для блоков
    allocator->memory = static_cast<char*>(realMemory) + sizeof(Allocator);
    allocator->memory_size = memory_size - sizeof(Allocator);
    allocator->used_memory = 0;

    // Инициализация списков свободных блоков
    for (size_t i = 0; i < 32; ++i) {
        allocator->free_lists[i] = nullptr;
    }

    // Инициализация первого блока
    BlockHeader* initial_block = static_cast<BlockHeader*>(allocator->memory);
    initial_block->size = allocator->memory_size;
    initial_block->is_free = true;
    initial_block->next = nullptr;

    // Добавляем первый блок в соответствующий список
    size_t index = static_cast<size_t>(std::log2(initial_block->size));
    initial_block->next = allocator->free_lists[index];
    allocator->free_lists[index] = initial_block;

    return allocator;
}

void* alloc(Allocator* allocator, size_t block_size) {
    if (block_size == 0) return nullptr;

    // Выравнивание размера запроса до степени двойки
    size_t aligned_size = 1;
    while (aligned_size < block_size) {
        aligned_size <<= 1;
    }

    size_t index = static_cast<size_t>(std::log2(aligned_size));

    // Ищем свободный блок в соответствующем списке
    if (allocator->free_lists[index]) {
        BlockHeader* block = allocator->free_lists[index];
        block->is_free = false;
        allocator->free_lists[index] = block->next; // Убираем блок из списка
        allocator->used_memory += aligned_size;
        return reinterpret_cast<void*>(block + 1);
    }

    
    for (size_t i = index + 1; i < 32; ++i) {
        if (allocator->free_lists[i]) {
            BlockHeader* block = allocator->free_lists[i];
            allocator->free_lists[i] = block->next; 
            // Разбиваем блок на два
            size_t current_size = 1 << i;
            while (current_size > aligned_size) {
                current_size >>= 1;
                BlockHeader* buddy = reinterpret_cast<BlockHeader*>(
                    reinterpret_cast<char*>(block) + current_size
                );
                buddy->size = current_size;
                buddy->is_free = true;
                size_t buddy_index = static_cast<size_t>(std::log2(current_size));
                buddy->next = allocator->free_lists[buddy_index];
                allocator->free_lists[buddy_index] = buddy;
            }

            block->size = aligned_size;
            block->is_free = false;
            allocator->used_memory += aligned_size;
            return reinterpret_cast<void*>(block + 1);
        }
    }

    return nullptr; 
}

void freeBlock(Allocator* allocator, void* block) {
    if (!block) return;

    BlockHeader* header = reinterpret_cast<BlockHeader*>(block) - 1;
    header->is_free = true;
    allocator->used_memory -= header->size;

    size_t index = static_cast<size_t>(std::log2(header->size));

    while (true) {
        size_t buddy_address = reinterpret_cast<size_t>(header) ^ (1 << index);
        BlockHeader* buddy = reinterpret_cast<BlockHeader*>(buddy_address);

        bool is_buddy_free = false;
        BlockHeader* prev = nullptr;
        BlockHeader* curr = allocator->free_lists[index];
        while (curr) {
            if (curr == buddy) {
                is_buddy_free = true;
                if (prev) {
                    prev->next = curr->next;
                } else {
                    allocator->free_lists[index] = curr->next;
                }
                break;
            }
            prev = curr;
            curr = curr->next;
        }

        if (!is_buddy_free) break;

        // Сливаем блоки
        if (header > buddy) {
            std::swap(header, buddy);
        }
        header->size *= 2;
        index++;
    }

    header->next = allocator->free_lists[index];
    allocator->free_lists[index] = header;
}
} 