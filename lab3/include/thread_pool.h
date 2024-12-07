#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <pthread.h>
#include <queue>
#include <vector>
#include <functional>

class ThreadPool {
public:
    explicit ThreadPool(size_t maxThreads);
    ~ThreadPool();

    void enqueueTask(std::function<void()> task);
    void waitForCompletion();

private:
    std::vector<pthread_t> workers;
    std::queue<std::function<void()>> tasks;
    pthread_mutex_t queueMutex;
    pthread_cond_t condition;
    size_t activeTasks;
    bool stop;

    static void* workerThread(void* arg);
    void processTasks();
    void taskCompleted();
};

#endif
