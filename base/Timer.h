#pragma once
#include <iostream>
#include <mutex>
#include <thread>
#include <functional>
#include <condition_variable>
#include <atomic>

class Timer {
public:
    Timer() {
        _expired = true;
        _tryToExpire = false;
    };
    ~Timer() {}

    void start(int interval, std::function<void()> task);
    void stop();

private:
    std::condition_variable _cv;
    std::mutex _mutex;

    std::atomic<bool> _expired;
    std::atomic<bool> _tryToExpire;
};

void Timer::start(int interval, std::function<void()> task) {
    if (_expired == false) {  // Timer 已经启动过了
        return;
    }

    _expired = false;

    std::thread([this, interval, task] () {
        while (!this->_tryToExpire) {
            std::this_thread::sleep_for(std::chrono::microseconds(interval));
            task();
        }

        std::lock_guard<std::mutex> lockGuard(this->_mutex);
        this->_expired = true;
        this->_cv.notify_one();
    }).detach();
}

void Timer::stop() {
    if (_expired == true)   return;
    if (_tryToExpire == true)   return;

    _tryToExpire = true;

    std::unique_lock<std::mutex> lockGuard(_mutex);
    _cv.wait(lockGuard, [this] { return this->_expired == true; });

    if (_expired == true) {
        _tryToExpire = false;
    }
}