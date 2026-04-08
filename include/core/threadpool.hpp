#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>


class ThreadPool{
    public:
         ThreadPool(size_t numThreads);
    ~ThreadPool();

    template<typename F>
    void enqueue(F&& task);

    void wait();
    private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;

    std::mutex queueMutex;
    std::condition_variable cv;
    std::atomic<int> tasksRemaining;
    std::condition_variable doneCv;
    std::mutex doneMutex;

    bool stop;
};

template<typename F>
void ThreadPool::enqueue(F&& f) {
    tasksRemaining++;
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        tasks.emplace([this, f]() {
            f();
            tasksRemaining--;
            doneCv.notify_one();
        });
    }
    cv.notify_one();
}