#include <iostream>
#include "../include/BuddyAllocator.h"
#include "../include/ListAllocator.h"

const size_t TEST_MEMORY_SIZE = 1024 * 1024; // 1 MB

void testBuddyAllocator() {
    char memory[TEST_MEMORY_SIZE];

    // Инициализация Buddy Allocator
    BuddyAllocator::Allocator* buddyAllocator = BuddyAllocator::createMemoryAllocator(memory, TEST_MEMORY_SIZE);
    if (!buddyAllocator) {
        std::cerr << "BuddyAllocator creation failed." << std::endl;
        return;
    }

    std::cout << "BuddyAllocator created successfully." << std::endl;

    // Тестирование выделения и освобождения памяти
    void* block1 = BuddyAllocator::alloc(buddyAllocator, 64);
    if (!block1) {
        std::cerr << "BuddyAllocator allocation failed." << std::endl;
        return;
    }

    std::cout << "BuddyAllocator allocated block of size 64 bytes." << std::endl;
    BuddyAllocator::freeBlock(buddyAllocator, block1);
    std::cout << "BuddyAllocator freed block." << std::endl;
}

void testListAllocator() {
    char memory[TEST_MEMORY_SIZE];

    // Инициализация List Allocator
    ListAllocator::Allocator* listAllocator = ListAllocator::createMemoryAllocator(memory, TEST_MEMORY_SIZE);
    if (!listAllocator) {
        std::cerr << "ListAllocator creation failed." << std::endl;
        return;
    }

    std::cout << "ListAllocator created successfully." << std::endl;

    // Тестирование выделения и освобождения памяти
    void* block1 = ListAllocator::alloc(listAllocator, 64);
    if (!block1) {
        std::cerr << "ListAllocator allocation failed." << std::endl;
        return;
    }

    std::cout << "ListAllocator allocated block of size 64 bytes." << std::endl;
    ListAllocator::freeBlock(listAllocator, block1);
    std::cout << "ListAllocator freed block." << std::endl;
}

int main() {
    // Тестирование Buddy Allocator
    testBuddyAllocator();

    // Тестирование List Allocator
    testListAllocator();

    return 0;
}
