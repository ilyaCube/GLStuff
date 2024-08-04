#pragma once

#include <memory>
#include <functional>
#include <thread>
#include "NonCopyable.hpp"

namespace Library::Common {

template<typename T>
class SafeContainer : NonCopyable
{
public:
    explicit SafeContainer() :
        _ptr(nullptr)
    {}

    explicit SafeContainer(const T& item) :
        _ptr(std::make_shared<T>(item))
    {}

    SafeContainer& operator=(const T& item)
    {
        reset();
        _ptr = std::make_shared<T>(item);
        return *this;
    }

    ~SafeContainer()
    {
        reset();
    }

    void reset(bool safe = true)
    {
        if (!_ptr) {
            return;
        }

        if (!safe) {
            _ptr = nullptr;
            return;
        }

        std::weak_ptr<T> weak = _ptr;
        _ptr = nullptr;

        // Block until callback will actually be dead (assure even that no current execution is performed)
        while (!weak.expired()) {
            std::this_thread::yield();
        }
    }

    explicit operator std::weak_ptr<T>() const
    {
        return weakPtr();
    }

    std::weak_ptr<T> weakPtr() const
    {
        return (std::weak_ptr<T>) _ptr;
    }

private:
    std::shared_ptr<T> _ptr;
};

using SafeCallback = Common::SafeContainer<std::function<void()>>;

}