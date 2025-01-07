#include "../CP/include/Allocator.h"
#include "../CP/include/BuddyAllocator.h"
#include "../CP/include/ListAllocator.h"
#include <gtest/gtest.h>
#include <cstdlib>
#include <vector>
#include <chrono>
#include <iostream>
#include <cstring> 

// Размер тестовой памяти
const size_t TEST_MEMORY_SIZE = 2 * 1024 * 1024; // 1 MB

// Вспомогательная функция для измерения времени
template<typename Func>
double measureTime(Func func) {
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    return duration.count();
}

// Вспомогательная функция для измерения использования памяти (в байтах)
size_t getHeapUsage(const Allocator* allocator) {
    return allocator->used_memory;
}

// Тест создания аллокаторов
TEST(AllocatorComparisonTest, CreationPerformance) {
    char listMemory[TEST_MEMORY_SIZE]; // Отдельная память для ListAllocator
    char buddyMemory[TEST_MEMORY_SIZE]; // Отдельная память для BuddyAllocator

    double listTime = measureTime([&]() {
        ListAllocator::Allocator* listAllocator = ListAllocator::createMemoryAllocator(listMemory, TEST_MEMORY_SIZE);
        ASSERT_NE(listAllocator, nullptr) << "ListAllocator creation failed.";
        std::cout << "Memory used by ListAllocator: " << getHeapUsage(listAllocator) << " bytes." << std::endl;
    });

    double buddyTime = measureTime([&]() {
        BuddyAllocator::Allocator* buddyAllocator = BuddyAllocator::createMemoryAllocator(buddyMemory, TEST_MEMORY_SIZE);
        ASSERT_NE(buddyAllocator, nullptr) << "BuddyAllocator creation failed.";
        std::cout << "Memory used by BuddyAllocator: " << getHeapUsage(buddyAllocator) << " bytes." << std::endl;
    });

    std::cout << "ListAllocator creation time: " << listTime << " seconds." << std::endl;
    std::cout << "BuddyAllocator creation time: " << buddyTime << " seconds." << std::endl;


}

// Тест для проверки поведения при нехватке памяти
TEST(AllocatorComparisonTest, OutOfMemory) {
    char listMemory[TEST_MEMORY_SIZE]; // Отдельная память для ListAllocator
    char buddyMemory[TEST_MEMORY_SIZE]; // Отдельная память для BuddyAllocator

    ListAllocator::Allocator* listAllocator = ListAllocator::createMemoryAllocator(listMemory, TEST_MEMORY_SIZE);
    ASSERT_NE(listAllocator, nullptr) << "ListAllocator creation failed.";

    BuddyAllocator::Allocator* buddyAllocator = BuddyAllocator::createMemoryAllocator(buddyMemory, TEST_MEMORY_SIZE);
    ASSERT_NE(buddyAllocator, nullptr) << "BuddyAllocator creation failed.";

    size_t blockSize = 64;
    size_t numBlocks = TEST_MEMORY_SIZE / blockSize;


    // Заполняем всю доступную память
    for (size_t i = 0; i < numBlocks; ++i) {
        void* listBlock = ListAllocator::alloc(listAllocator, blockSize);
        ASSERT_NE(listBlock, nullptr) << "ListAllocator allocation failed at block " << i;

        void* buddyBlock = BuddyAllocator::alloc(buddyAllocator, blockSize);
        if (buddyBlock == nullptr) {
            std::cout << "BuddyAllocator out of memory at block " << i << std::endl;
            break;
        }
        ASSERT_NE(buddyBlock, nullptr) << "BuddyAllocator allocation failed at block " << i;
    }

}
// Тест выделения и освобождения
TEST(AllocatorComparisonTest, AllocationAndFreePerformance) {
    char listMemory[TEST_MEMORY_SIZE]; // Отдельная память для ListAllocator
    char buddyMemory[TEST_MEMORY_SIZE]; // Отдельная память для BuddyAllocator

    ListAllocator::Allocator* listAllocator = ListAllocator::createMemoryAllocator(listMemory, TEST_MEMORY_SIZE);
    ASSERT_NE(listAllocator, nullptr) << "ListAllocator creation failed.";

    BuddyAllocator::Allocator* buddyAllocator = BuddyAllocator::createMemoryAllocator(buddyMemory, TEST_MEMORY_SIZE);
    ASSERT_NE(buddyAllocator, nullptr) << "BuddyAllocator creation failed.";

    const size_t numBlocks = 1000;
    const size_t blockSize = 64;

    double listAllocTime = measureTime([&]() {
        for (size_t i = 0; i < numBlocks; ++i) {
            void* block = ListAllocator::alloc(listAllocator, blockSize);
            ASSERT_NE(block, nullptr) << "ListAllocator allocation failed at block " << i;
            ListAllocator::freeBlock(listAllocator, block);
        }
    });

    double buddyAllocTime = measureTime([&]() {
        for (size_t i = 0; i < numBlocks; ++i) {
            void* block = BuddyAllocator::alloc(buddyAllocator, blockSize);
            ASSERT_NE(block, nullptr) << "BuddyAllocator allocation failed at block " << i;
            BuddyAllocator::freeBlock(buddyAllocator, block);
        }
    });

    std::cout << "ListAllocator allocation/free time: " << listAllocTime << " seconds." << std::endl;
    std::cout << "BuddyAllocator allocation/free time: " << buddyAllocTime << " seconds." << std::endl;


}

// Тест для проверки выделения и освобождения блоков разного размера
TEST(AllocatorComparisonTest, MixedBlockSizes) {
    char listMemory[TEST_MEMORY_SIZE]; // Отдельная память для ListAllocator
    char buddyMemory[TEST_MEMORY_SIZE]; // Отдельная память для BuddyAllocator

    ListAllocator::Allocator* listAllocator = ListAllocator::createMemoryAllocator(listMemory, TEST_MEMORY_SIZE);
    ASSERT_NE(listAllocator, nullptr) << "ListAllocator creation failed.";

    BuddyAllocator::Allocator* buddyAllocator = BuddyAllocator::createMemoryAllocator(buddyMemory, TEST_MEMORY_SIZE);
    ASSERT_NE(buddyAllocator, nullptr) << "BuddyAllocator creation failed.";

    std::vector<size_t> blockSizes = {32, 64, 128, 256, 512, 1024};

    for (size_t size : blockSizes) {
        void* listBlock = ListAllocator::alloc(listAllocator, size);
        ASSERT_NE(listBlock, nullptr) << "ListAllocator allocation failed for size " << size;
        ListAllocator::freeBlock(listAllocator, listBlock);

        void* buddyBlock = BuddyAllocator::alloc(buddyAllocator, size);
        ASSERT_NE(buddyBlock, nullptr) << "BuddyAllocator allocation failed for size " << size;
        BuddyAllocator::freeBlock(buddyAllocator, buddyBlock);
    }

}

// Тест для проверки повторного использования освобожденных блоков
TEST(AllocatorComparisonTest, ReuseFreedBlocks) {
    char listMemory[TEST_MEMORY_SIZE]; // Отдельная память для ListAllocator
    char buddyMemory[TEST_MEMORY_SIZE]; // Отдельная память для BuddyAllocator

    ListAllocator::Allocator* listAllocator = ListAllocator::createMemoryAllocator(listMemory, TEST_MEMORY_SIZE);
    ASSERT_NE(listAllocator, nullptr) << "ListAllocator creation failed.";

    BuddyAllocator::Allocator* buddyAllocator = BuddyAllocator::createMemoryAllocator(buddyMemory, TEST_MEMORY_SIZE);
    ASSERT_NE(buddyAllocator, nullptr) << "BuddyAllocator creation failed.";

    size_t blockSize = 64;

    // Выделяем блоки
    void* listBlock1 = ListAllocator::alloc(listAllocator, blockSize);
    ASSERT_NE(listBlock1, nullptr) << "ListAllocator allocation failed.";
    void* buddyBlock1 = BuddyAllocator::alloc(buddyAllocator, blockSize);
    ASSERT_NE(buddyBlock1, nullptr) << "BuddyAllocator allocation failed.";

    // Освобождаем блоки
    ListAllocator::freeBlock(listAllocator, listBlock1);
    BuddyAllocator::freeBlock(buddyAllocator, buddyBlock1);

    // Выделяем блоки снова
    void* listBlock2 = ListAllocator::alloc(listAllocator, blockSize);
    ASSERT_NE(listBlock2, nullptr) << "ListAllocator failed to reuse freed block.";
    void* buddyBlock2 = BuddyAllocator::alloc(buddyAllocator, blockSize);
    ASSERT_NE(buddyBlock2, nullptr) << "BuddyAllocator failed to reuse freed block.";

    // Освобождаем блоки
    ListAllocator::freeBlock(listAllocator, listBlock2);
    BuddyAllocator::freeBlock(buddyAllocator, buddyBlock2);

}
// Тест большого числа блоков
TEST(AllocatorComparisonTest, LargeNumberOfBlocks) {
    char listMemory[TEST_MEMORY_SIZE]; // Отдельная память для ListAllocator
    char buddyMemory[TEST_MEMORY_SIZE]; // Отдельная память для BuddyAllocator

    ListAllocator::Allocator* listAllocator = ListAllocator::createMemoryAllocator(listMemory, TEST_MEMORY_SIZE);
    BuddyAllocator::Allocator* buddyAllocator = BuddyAllocator::createMemoryAllocator(buddyMemory, TEST_MEMORY_SIZE);

    const size_t numBlocks = 10000;
    const size_t blockSize = 64;

    double listTime = measureTime([&]() {
        for (size_t i = 0; i < numBlocks; ++i) {
            void* block = ListAllocator::alloc(listAllocator, blockSize);
            ASSERT_NE(block, nullptr);
            ListAllocator::freeBlock(listAllocator, block);
        }
    });

    double buddyTime = measureTime([&]() {
        for (size_t i = 0; i < numBlocks; ++i) {
            void* block = BuddyAllocator::alloc(buddyAllocator, blockSize);
            ASSERT_NE(block, nullptr);
            BuddyAllocator::freeBlock(buddyAllocator, block);
        }
    });

    std::cout << "ListAllocator time for " << numBlocks << " blocks: " << listTime << " seconds." << std::endl;
    std::cout << "BuddyAllocator time for " << numBlocks << " blocks: " << buddyTime << " seconds." << std::endl;


}

// Тест для проверки производительности при работе с блоками размером 2^n
TEST(AllocatorComparisonTest, PowerOfTwoBlocks) {
    char listMemory[TEST_MEMORY_SIZE]; // Отдельная память для ListAllocator
    char buddyMemory[TEST_MEMORY_SIZE]; // Отдельная память для BuddyAllocator

    ListAllocator::Allocator* listAllocator = ListAllocator::createMemoryAllocator(listMemory, TEST_MEMORY_SIZE);
    ASSERT_NE(listAllocator, nullptr) << "ListAllocator creation failed.";

    BuddyAllocator::Allocator* buddyAllocator = BuddyAllocator::createMemoryAllocator(buddyMemory, TEST_MEMORY_SIZE);
    ASSERT_NE(buddyAllocator, nullptr) << "BuddyAllocator creation failed.";

    const size_t numBlocks = 1000;
    std::vector<size_t> blockSizes = {32, 64, 128, 256, 512, 1024}; // Блоки размером 2^n

    double listTime = measureTime([&]() {
        for (size_t i = 0; i < numBlocks; ++i) {
            for (size_t size : blockSizes) {
                void* block = ListAllocator::alloc(listAllocator, size);
                ASSERT_NE(block, nullptr) << "ListAllocator allocation failed for size " << size;
                ListAllocator::freeBlock(listAllocator, block);
            }
        }
    });

    double buddyTime = measureTime([&]() {
        for (size_t i = 0; i < numBlocks; ++i) {
            for (size_t size : blockSizes) {
                void* block = BuddyAllocator::alloc(buddyAllocator, size);
                ASSERT_NE(block, nullptr) << "BuddyAllocator allocation failed for size " << size;
                BuddyAllocator::freeBlock(buddyAllocator, block);
            }
        }
    });

    std::cout << "ListAllocator time for " << numBlocks << " blocks of power-of-two sizes: " << listTime << " seconds." << std::endl;
    std::cout << "BuddyAllocator time for " << numBlocks << " blocks of power-of-two sizes: " << buddyTime << " seconds." << std::endl;

}

TEST(AllocatorComparisonTest, MemoryUsageCheck) {
    char listMemory[TEST_MEMORY_SIZE]; // Отдельная память для ListAllocator
    char buddyMemory[TEST_MEMORY_SIZE]; // Отдельная память для BuddyAllocator

    ListAllocator::Allocator* listAllocator = ListAllocator::createMemoryAllocator(listMemory, TEST_MEMORY_SIZE);
    ASSERT_NE(listAllocator, nullptr) << "ListAllocator creation failed.";

    BuddyAllocator::Allocator* buddyAllocator = BuddyAllocator::createMemoryAllocator(buddyMemory, TEST_MEMORY_SIZE);
    ASSERT_NE(buddyAllocator, nullptr) << "BuddyAllocator creation failed.";

    size_t blockSize = 64;

    // Выделяем блок
    void* listBlock = ListAllocator::alloc(listAllocator, blockSize);
    ASSERT_NE(listBlock, nullptr) << "ListAllocator allocation failed.";
    ASSERT_EQ(getHeapUsage(listAllocator), blockSize) << "ListAllocator memory usage is incorrect.";

    void* buddyBlock = BuddyAllocator::alloc(buddyAllocator, blockSize);
    ASSERT_NE(buddyBlock, nullptr) << "BuddyAllocator allocation failed.";
    ASSERT_EQ(getHeapUsage(buddyAllocator), blockSize) << "BuddyAllocator memory usage is incorrect.";

    // Освобождаем блок
    ListAllocator::freeBlock(listAllocator, listBlock);
    ASSERT_EQ(getHeapUsage(listAllocator), 0) << "ListAllocator memory usage is incorrect after free.";

    BuddyAllocator::freeBlock(buddyAllocator, buddyBlock);
    ASSERT_EQ(getHeapUsage(buddyAllocator), 0) << "BuddyAllocator memory usage is incorrect after free.";

    // Очистка памяти
    memset(listMemory, 0, TEST_MEMORY_SIZE);
    memset(buddyMemory, 0, TEST_MEMORY_SIZE);
}

TEST(AllocatorComparisonTest, BuddyAllocatorPerformance) {
    const size_t TEST_MEMORY_SIZE = 1024 * 1024 * 1024; // 1 GB (увеличьте размер памяти при необходимости)
    
    // Выделяем память на куче вместо стека
    char* listMemory = new char[TEST_MEMORY_SIZE];
    char* buddyMemory = new char[TEST_MEMORY_SIZE];

    ListAllocator::Allocator* listAllocator = ListAllocator::createMemoryAllocator(listMemory, TEST_MEMORY_SIZE);
    ASSERT_NE(listAllocator, nullptr) << "ListAllocator creation failed.";

    BuddyAllocator::Allocator* buddyAllocator = BuddyAllocator::createMemoryAllocator(buddyMemory, TEST_MEMORY_SIZE);
    ASSERT_NE(buddyAllocator, nullptr) << "BuddyAllocator creation failed.";

    const size_t numBlocks = 10000; // Количество блоков
    const size_t maxBlockSize = 1024 * 1024; // Максимальный размер блока (1 MB)
    std::vector<size_t> blockSizes(numBlocks);

    // Генерация случайных размеров блоков
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    for (size_t i = 0; i < numBlocks; ++i) {
        blockSizes[i] = (std::rand() % maxBlockSize) + 1; // Размеры от 1 до maxBlockSize
    }

    // Тест для ListAllocator
    size_t listFailures = 0;
    double listTime = measureTime([&]() {
        for (size_t i = 0; i < numBlocks; ++i) {
            void* block = ListAllocator::alloc(listAllocator, blockSizes[i]);
            if (block == nullptr) {
                listFailures++;
            } else {
                ListAllocator::freeBlock(listAllocator, block);
            }
        }
    });

    // Тест для BuddyAllocator
    size_t buddyFailures = 0;
    double buddyTime = measureTime([&]() {
        for (size_t i = 0; i < numBlocks; ++i) {
            void* block = BuddyAllocator::alloc(buddyAllocator, blockSizes[i]);
            if (block == nullptr) {
                buddyFailures++;
            } else {
                BuddyAllocator::freeBlock(buddyAllocator, block);
            }
        }
    });

    std::cout << "ListAllocator time for " << numBlocks << " random-sized blocks: " << listTime << " seconds. Failures: " << listFailures << std::endl;
    std::cout << "BuddyAllocator time for " << numBlocks << " random-sized blocks: " << buddyTime << " seconds. Failures: " << buddyFailures << std::endl;

    // Ожидаем, что BuddyAllocator будет быстрее
    EXPECT_LT(buddyTime, listTime) << "BuddyAllocator should be faster than ListAllocator.";

    // Проверяем, что количество сбоев BuddyAllocator не превышает допустимого предела
    EXPECT_LE(buddyFailures, 0) << "BuddyAllocator had too many allocation failures.";

    // Освобождаем память
    delete[] listMemory;
    delete[] buddyMemory;
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}