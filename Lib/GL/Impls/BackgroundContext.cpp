#include "BackgroundContext.hpp"

namespace Library::GL::Impls {

BackgroundContext::BackgroundContext(QThread::Priority priority) 
    : Threading::BackgroundOwnerAndContext(priority),
    _openGLSurface(nullptr),
    _openGLContext(nullptr)
{
    // Create (and destroy) surface on main thread
    _openGLSurface = new QOffscreenSurface();
    _openGLSurface->create();

    invokeSync([&]() {
        _openGLContext = createOpenGLContext();
    });
}

BackgroundContext::~BackgroundContext()
{
    invokeSync([&]() {
        _thread.quit();

        delete _openGLContext;
        _openGLContext = nullptr;
    });

    delete _openGLSurface;
    _openGLSurface = nullptr;
}

QOpenGLContext *BackgroundContext::createOpenGLContext()
{
    auto context = new QOpenGLContext();

    context->setFormat(_openGLSurface->format());
    context->setShareContext(QOpenGLContext::globalShareContext());
    context->create();
    context->makeCurrent(_openGLSurface);

    return context;
}

}