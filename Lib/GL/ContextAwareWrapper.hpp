#pragma once

#include <utility>
#include <type_traits>
#include <functional>
#include <QObject>
#include <QMetaObject>
#include "Common/NonCopyable.hpp"

namespace Library::GL {

template<typename T>
class ContextAwareWrapper : private Common::NonCopyable
{
public:
    template<typename ...Args>
    explicit ContextAwareWrapper(Args&& ...args) :
        _constructor([args...]() {
            return new T(args...);
        })
    {}

    ~ContextAwareWrapper()
    {
        if (!_object) {
            return;
        }

        QMetaObject::invokeMethod(_context, [context = _context, object = _object] {
            delete context;
            delete object;
        });
    }

    T *get() const
    {
        if (!_object) {
            _object = _constructor();
            _context = new QObject();
        }

        return _object;
    }

    inline T& operator*() const
    {
        return get();
    }

    inline T *operator->() const
    {
        return get();
    }

private:
    const std::function<T *()> _constructor;
    mutable T *_object{nullptr};
    mutable QObject *_context{nullptr};
};

}