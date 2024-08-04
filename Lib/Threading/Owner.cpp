#include "Owner.hpp"

namespace Library::Threading {

Owner::~Owner()
{
    for (const auto& deleter : _deleters) {
        deleter();
    }
}

}