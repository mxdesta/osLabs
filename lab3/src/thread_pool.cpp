#include "thread_pool.h"
#include <stdexcept>

ThreadPool::ThreadPool(size_t maxThreads) : activeTasks(0), stop(false) {
    pthread_mutex_init(&queueMutex, nullptr);
    pthread_cond_init(&condition, nullptr);

    for (size_t i = 0; i < maxThreads; ++i) {
        pthread_t thread;
        if (pthread_create(&thread, nullptr, workerThread, this) != 0) {
            throw std::runtime_error("Failed to create thread");
        }
        workers.push_back(thread);
    }
}


ThreadPool::~ThreadPool() {
    {
        pthread_mutex_lock(&queueMutex);
        stop = true;
        pthread_mutex_unlock(&queueMutex);
        pthread_cond_broadcast(&condition);
    }

    for (pthread_t &worker : workers) {
        pthread_join(worker, nullptr);
    }

    pthread_mutex_destroy(&queueMutex);
    pthread_cond_destroy(&condition);
}

void ThreadPool::enqueueTask(std::function<void()> task) {
    pthread_mutex_lock(&queueMutex);
    tasks.push(std::move(task));
    ++activeTasks;
    pthread_mutex_unlock(&queueMutex);
    pthread_cond_signal(&condition);
}

void ThreadPool::taskCompleted() {
    pthread_mutex_lock(&queueMutex);
    --activeTasks;
    if (activeTasks == 0 && tasks.empty()) {
        pthread_cond_broadcast(&condition);
    }
    pthread_mutex_unlock(&queueMutex);
}

void ThreadPool::waitForCompletion() {
    pthread_mutex_lock(&queueMutex);
    while (activeTasks > 0 || !tasks.empty()) {
        pthread_cond_wait(&condition, &queueMutex);
    }
    pthread_mutex_unlock(&queueMutex);
}

void* ThreadPool::workerThread(void* arg) {
    auto* pool = static_cast<ThreadPool*>(arg);
    pool->processTasks();
    return nullptr;
}

void ThreadPool::processTasks() {
    while (true) {
        std::function<void()> task;
        {
            pthread_mutex_lock(&queueMutex);
            while (!stop && tasks.empty()) {
                pthread_cond_wait(&condition, &queueMutex);
            }
            if (stop && tasks.empty()) {
                pthread_mutex_unlock(&queueMutex);
                return;
            }
            task = std::move(tasks.front());
            tasks.pop();
            pthread_mutex_unlock(&queueMutex);
        }
        task();
        taskCompleted();
    }
}
