#include "Context.hpp"

namespace Library::Threading {

void Context::invokeQueuedWeak(const std::weak_ptr<std::function<void()>>& weakCallback) const
{
    invokeImpl(
        [=]() {
            if (auto callback = weakCallback.lock()) {
                (*callback)();
            }
        },
        false
    );
}

void Context::invokeAutoWeak(const std::weak_ptr<std::function<void()>>& weakCallback) const
{
    invokeImpl(
        [=]() {
            if (auto callback = weakCallback.lock()) {
                (*callback)();
            }
        },
        true
    );
}

}