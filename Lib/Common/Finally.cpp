#include "Finally.hpp"

namespace Library::Common {

Finally::Finally(const std::function<void()>& finalize) :
    _finalize(finalize)
{
}

Finally::~Finally()
{
    if (_enabled) {
        _finalize();
    }
}

void Finally::disable()
{
    _enabled = false;
}

}