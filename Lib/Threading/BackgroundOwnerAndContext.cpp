#include "BackgroundOwnerAndContext.hpp"

namespace Library::Threading {

BackgroundOwnerAndContext::BackgroundOwnerAndContext(QThread::Priority priority)
{
    this->moveToThread(&_thread);
    _thread.start(priority);
}

BackgroundOwnerAndContext::~BackgroundOwnerAndContext()
{
    _thread.quit();
    _thread.wait();
}

void BackgroundOwnerAndContext::invokeImpl(const std::function<void()>& func, bool allowImmediateExec) const
{
    QMetaObject::invokeMethod(
        (QObject*)this,
        func,
        allowImmediateExec ? Qt::AutoConnection : Qt::QueuedConnection
    );
}

}
