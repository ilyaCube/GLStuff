#pragma once

#include <memory>
#include <functional>
#include <exception>
#include "Latch.hpp"

namespace Library::Threading {

class Context
{
public:
    Context() = default;
    virtual ~Context() = default;

    void invokeQueuedWeak(const std::weak_ptr<std::function<void()>>& weakCallback) const;

    void invokeAutoWeak(const std::weak_ptr<std::function<void()>>& weakCallback) const;

    template<typename T>
    void invokeQueuedWeak(const std::weak_ptr<std::function<void(const T&)>>& weakCallback, const T& arg) const
    {
        invokeImpl(
            [=]() {
                if (auto callback = weakCallback.lock()) {
                    (*callback)(arg);
                }
            },
            false
        );
    }

    template<typename T>
    void invokeAutoWeak(const std::weak_ptr<std::function<void(const T&)>>& weakCallback, const T& arg) const
    {
        invokeImpl(
            [=]() {
                if (auto callback = weakCallback.lock()) {
                    (*callback)(arg);
                }
            },
            true
        );
    }

    template<typename Func, typename ...Args>
    inline void invokeQueued(Func&& func, Args&& ... args) const
    {
        invokeImpl([func, args...]() { func(args...); }, false);
    }

    template<typename Func, typename ...Args>
    inline void invokeAuto(Func&& func, Args&& ... args) const
    {
        invokeImpl([func, args...]() { func(args...); }, true);
    }

    template<typename Func, typename ...Args>
    void invokeSync(Func&& func, Args&& ... args) const
    {
        std::exception_ptr _exceptionPtr{nullptr};
        Threading::Latch latch(1);

        invokeImpl([&]() {
            try {
                func(args...);
            }
            catch (...) {
                _exceptionPtr = std::current_exception();
            }

            latch.countDown();
        }, true);

        latch.wait();
        if (_exceptionPtr) {
            std::rethrow_exception(_exceptionPtr);
        }
    }

protected:
    virtual void invokeImpl(const std::function<void()>& func, bool allowImmediateExec) const = 0;
};

}
