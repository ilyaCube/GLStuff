#pragma once
#include "Impls/BackgroundContext.hpp"

namespace Library::GL {

using BackgroundContext = std::shared_ptr<Impls::BackgroundContext>;

inline BackgroundContext createContext(QThread::Priority priority = QThread::HighPriority)
{
    return std::make_shared<Impls::BackgroundContext>(priority);
}

}