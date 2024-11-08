#include "quicksort.h"

QuickSort::QuickSort(size_t maxThreads) : threadPool(maxThreads) {}

void QuickSort::sort(std::vector<int>& arr) {
    parallelQuickSort(arr, 0, arr.size() - 1);
    threadPool.waitForCompletion();
}

void QuickSort::parallelQuickSort(std::vector<int>& arr, int left, int right) {
    if (left < right) {
        int pivotIndex = partition(arr, left, right);
        int pivotIndexLeft = pivotIndex - 1;
        int pivotIndexRight = pivotIndex + 1;

        if ((right - left) > 1000) {
            threadPool.enqueueTask([this, &arr, left, pivotIndexLeft]() {
                parallelQuickSort(arr, left, pivotIndexLeft);
            });
            threadPool.enqueueTask([this, &arr, pivotIndexRight, right]() {
                parallelQuickSort(arr, pivotIndexRight, right);
            });
        } else {
            parallelQuickSort(arr, left, pivotIndexLeft);
            parallelQuickSort(arr, pivotIndexRight, right);
        }
    }
}

int QuickSort::partition(std::vector<int>& arr, int left, int right) {
    int pivot = arr[right];
    int i = left - 1;
    for (int j = left; j <= right - 1; ++j) {
        if (arr[j] < pivot) {
            ++i;
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[right]);
    return i + 1;
}
