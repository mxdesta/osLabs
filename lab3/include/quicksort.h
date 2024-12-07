#ifndef QUICKSORT_H
#define QUICKSORT_H

#include <vector>
#include "thread_pool.h"

class QuickSort {
public:
    explicit QuickSort(size_t maxThreads);
    void sort(std::vector<int>& arr);

private:
    ThreadPool threadPool;
    void parallelQuickSort(std::vector<int>& arr, int left, int right);
    int partition(std::vector<int>& arr, int left, int right);
};

#endif
