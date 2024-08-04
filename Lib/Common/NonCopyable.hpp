#pragma once

namespace Library::Common {

struct NonCopyable
{
    NonCopyable(const NonCopyable&) = delete;
    const NonCopyable& operator=(const NonCopyable&) = delete;

protected:
    NonCopyable() = default;
};

}
