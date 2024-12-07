#include <iostream>
#include <vector>
#include <cstdlib>
#include <chrono>
#include "include/quicksort.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <number of threads>" << std::endl;
        return 1;
    }

    size_t maxThreads = std::stoi(argv[1]);
    size_t arraySize;
    std::cout << "Enter the size of the array: ";
    std::cin >> arraySize;

    std::vector<int> arr(arraySize);
    std::cout << "Enter the elements of the array: ";
    for (size_t i = 0; i < arraySize; ++i) {
        std::cin >> arr[i];
    }

    QuickSort sorter(maxThreads);

    auto start = std::chrono::high_resolution_clock::now();
    sorter.sort(arr);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    std::cout << "Sorted array: ";
    for (const auto& num : arr) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    std::cout << "Sorting completed in " << duration.count() << " seconds." << std::endl;

    return 0;
}
