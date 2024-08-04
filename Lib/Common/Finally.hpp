#pragma once

#include <functional>
#include "NonCopyable.hpp"

namespace Library::Common {

class Finally : private Common::NonCopyable
{
public:
    explicit Finally(const std::function<void()>& finalize);

    ~Finally();

    void disable();

private:
    const std::function<void()> _finalize;
    bool _enabled{true};
};

}
