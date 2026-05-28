#ifndef _H_THREAD_POOL_H_
#define _H_THREAD_POOL_H_

#include <future>
#include <thread>
#include <mutex>
#include <functional>
#include <queue>
class ThreadPool {
public: 
    ThreadPool(size_t numThreads);
    ~ThreadPool();
    
    template <typename F>
    std::future<std::invoke_result_t<F>> submit(F&& task) {
        using ReturnType = std::invoke_result_t<F>;
        auto packagedTask = std::make_shared<std::packaged_task<ReturnType()>>(std::forward<F>(task));
        std::future<ReturnType> result = packagedTask->get_future();
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            if (stop) {
                throw std::runtime_error("ThreadPool is stopped");
            }
            tasks.emplace([packagedTask]() { (*packagedTask)(); });
        }
        condition.notify_one();
        return result;
    }
private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queueMutex;
    std::condition_variable condition;
    bool stop;
};

#endif // _H_THREAD_POOL_H_