#pragma once

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>

class ThreadPool {
public:
    ThreadPool(size_t);
    template<typename F, typename... Args>
    auto enqueue(F&& f, Args&&... args)
        -> std::future<typename std::result_of<F(Args...)>::type>; // result_of 的作用与 decltype 相同，不国针对的是函数返回值
private:
    std::vector<std::thread> _workers;
    std::queue<std::function<void()>> _tasks;

    std::mutex _queueMutex;
    std::condition_variable _condition;
    bool _stop;
};


inline ThreadPool::ThreadPool(size_t threads) : _stop(false) {
    for(size_t i = 0; i < threads; ++i) {
        _workers.emplace_back([this] {
            while(true) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(this->_queueMutex);
                    this->_condition.wait(lock, 
                                        [this]{ return this->_stop || !this->_tasks.empty(); });
                    if(this->_stop && this->_tasks.empty())
                        return;
                    task = std::move(this->_tasks.front()); 
                    this->_tasks.pop();
                }

                task();
            }
        });
    }
}