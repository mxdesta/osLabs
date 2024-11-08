#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include <chrono>
#include "../include/quicksort.h"

TEST(QuickSortTest, SingleThreadCorrectness) {
    std::vector<int> input = {10, 3, 5, 2, 8, 6, 4, 7, 9, 1};
    std::vector<int> expected = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    QuickSort sorter(1);
    sorter.sort(input);

    EXPECT_EQ(input, expected);
}

TEST(QuickSortTest, SingleVsMultiThreadCorrectness) {
    std::vector<int> input = {15, 3, 8, 10, 1, 14, 6, 7, 12, 2};

    std::vector<int> expected = input;
    std::sort(expected.begin(), expected.end());

    std::vector<int> singleThreadSorted = input;
    QuickSort singleThreadSorter(1);
    singleThreadSorter.sort(singleThreadSorted);

    std::vector<int> multiThreadSorted = input;
    QuickSort multiThreadSorter(4);
    multiThreadSorter.sort(multiThreadSorted);
    EXPECT_EQ(singleThreadSorted, multiThreadSorted);
    EXPECT_EQ(singleThreadSorted, expected);
}

TEST(QuickSortTest, MultiThreadPerformance) {
    size_t size = 1000000; 
    std::vector<int> arr(size);
    for (auto& num : arr) {
        num = rand() % 10000;
    }

    std::vector<int> arrSingleThread = arr;
    std::vector<int> arrMultiThread = arr;

    QuickSort singleThreadSorter(1);
    auto startSingle = std::chrono::high_resolution_clock::now();
    singleThreadSorter.sort(arrSingleThread);
    auto endSingle = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> singleThreadDuration = endSingle - startSingle;
    
    QuickSort multiThreadSorter(8);
    auto startMulti = std::chrono::high_resolution_clock::now();
    multiThreadSorter.sort(arrMultiThread);
    auto endMulti = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> multiThreadDuration = endMulti - startMulti;

    EXPECT_TRUE(std::is_sorted(arrSingleThread.begin(), arrSingleThread.end()));
    EXPECT_TRUE(std::is_sorted(arrMultiThread.begin(), arrMultiThread.end()));
    EXPECT_EQ(arrSingleThread, arrMultiThread);

    EXPECT_TRUE(multiThreadDuration.count() < singleThreadDuration.count());
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}