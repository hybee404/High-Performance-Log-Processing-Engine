#include "core/threadpool.hpp"

ThreadPool::ThreadPool(size_t numThreads) : stop(false), tasksRemaining(0) {
    for (size_t i = 0; i < numThreads; ++i) {
        workers.emplace_back([this]() {
            while (true) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(queueMutex);
                    cv.wait(lock, [this]() { return stop || !tasks.empty(); });

                    if (stop && tasks.empty())
                        return;
                         task = std::move(tasks.front());
                    tasks.pop();
                }
                task();
            }
        });
    }
}

void ThreadPool::wait() {
    std::unique_lock<std::mutex> lock(doneMutex);
    doneCv.wait(lock, [this]() {
        return tasksRemaining == 0;
    });
}

ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        stop = true;
    }
    cv.notify_all();

    for (auto& t : workers)
        t.join();
}

