#pragma once

#include <QOffscreenSurface>
#include <QOpenGLContext>
#include "Threading/BackgroundOwnerAndContext.hpp"

namespace Library::GL::Impls {

class BackgroundContext : public Threading::BackgroundOwnerAndContext
{
public:
    explicit BackgroundContext(QThread::Priority priority);
    ~BackgroundContext() override;

private:
    QOpenGLContext *createOpenGLContext();

    QOffscreenSurface *_openGLSurface;
    QOpenGLContext *_openGLContext;
};

}