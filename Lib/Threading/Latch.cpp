#include "Latch.hpp"

namespace Library::Threading {

Latch::Latch(int count) :
    _counter(count)
{
}

void Latch::countDown()
{
    if (--_counter <= 0) {
        _cv.notify_all();
    }
}

void Latch::countDownAndWait()
{
    countDown();
    wait();
}

void Latch::wait() const
{
    if (_counter > 0) {
        std::unique_lock<std::mutex> lock(_mutex);
        _cv.wait(lock, [&]() { return _counter <= 0; });
    }
}

bool Latch::isFinished() const
{
    return _counter <= 0;
}

}