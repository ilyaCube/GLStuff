#pragma once

#include <functional>
#include <QThread>
#include "OwnerAndContext.hpp"

namespace Library::Threading {

class BackgroundOwnerAndContext : public QObject, public OwnerAndContext
{
public:
    explicit BackgroundOwnerAndContext(QThread::Priority priority = QThread::InheritPriority);
    ~BackgroundOwnerAndContext() override;

protected:
    QThread _thread;

    void invokeImpl(const std::function<void()>& func, bool allowImmediateExec) const override;
};

}