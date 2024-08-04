#pragma once

#include <vector>
#include <functional>

namespace Library::Threading {

class Owner
{
public:
    Owner() = default;
    virtual ~Owner();

    template<typename T>
    void add(const T *obj)
    {
        _deleters.emplace_back([obj]() {
            delete obj;
        });
    }

private:
    std::vector<std::function<void()>> _deleters;
};

}
