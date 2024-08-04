#pragma once

#include <atomic>
#include <mutex>
#include <condition_variable>
#include "Common/NonCopyable.hpp"

namespace Library::Threading {

class Latch : private Common::NonCopyable
{
public:
    explicit Latch(int count);

    void countDown();
    void countDownAndWait();
    void wait() const;
    bool isFinished() const;

private:
    std::atomic_int _counter;
    mutable std::condition_variable _cv;
    mutable std::mutex _mutex;
};

}