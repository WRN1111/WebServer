//
// Created by wrn on 5/12/23.
//

#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <mutex>
#include <condition_variable>
#include <queue>
#include <thread>
#include <functional>

class ThreadPool {

public:
    explicit ThreadPool(size_t threadCount = 8) : _pool(std::make_shared<Pool>()) {
        for (size_t i = 0; i < threadCount; i++) {
            std::thread([pool = _pool] {
                std::unique_lock<std::mutex> locker(pool->mutex);
                while (true) {
                    if (!pool->tasks.empty()) {
                        auto task = std::move(pool->tasks.front());
                        pool->tasks.pop();
                        locker.unlock();
                        task();
                        locker.lock();
                    } else if (pool->isClosed) break;
                    else pool->cond.wait(locker);
                }
            }).detach();
        }

    }

    ThreadPool() = default;

    ThreadPool(ThreadPool &&) = default;

    ~ThreadPool() {
        if (static_cast<bool>(_pool)) {
            {
                std::lock_guard<std::mutex> locker(_pool->mutex);
                _pool->isClosed = true;
            }
            _pool->cond.notify_all();
        }
    }

    template<class F>
    void AddTask(F &&task) {
        {
            std::lock_guard<std::mutex> locker(_pool->mutex);
            _pool->tasks.emplace(std::forward<F>(task));
        }
        _pool->cond.notify_one();
    }

private:
    struct Pool {
        std::mutex mutex;
        std::condition_variable cond;
        bool isClosed;
        std::queue<std::function<void()>> tasks;
    };
    std::shared_ptr<Pool> _pool;
};


#endif //LINUX_THREADPOLL_H
