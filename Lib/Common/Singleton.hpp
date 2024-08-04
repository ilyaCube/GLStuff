#pragma once

#include "NonCopyable.hpp"

namespace Library::Common {

template <typename T>
class Singleton : private NonCopyable
{
public:
    inline static T& instance()
    {
        static T instance;
        return instance;
    }

    inline static T* ptr()
    {
        return &instance();
    }
};

}