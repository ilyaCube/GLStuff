#include "OpenGLRenderer.hpp"

#include <QtQuick/QQuickWindow>
#include <QOpenGLFramebufferObjectFormat>
#include "GL/GL.hpp"

namespace Library::Qml {

Renderer::Renderer(const OpenGLPaintedItem *view)
    : _view(view),
    _weakMutex(view->weakMutex())
{
}

QOpenGLFramebufferObject *Renderer::createFramebufferObject(const QSize& size_px)
{
    auto mutexShared = _weakMutex.lock();
    if (mutexShared != nullptr) {
        std::lock_guard<std::mutex> lock(*mutexShared);
        if (auto wnd = _view->window()) {
            auto dpi = wnd->effectiveDevicePixelRatio();
            _viewport = size_px / dpi;
            return _view->createFramebufferObject(size_px);
        }
    }

    // Fallback (should never happen)
    return new QOpenGLFramebufferObject(size_px, QOpenGLFramebufferObjectFormat());
}

void Renderer::render()
{
    auto mutexShared = _weakMutex.lock();
    if(mutexShared == nullptr) {
        return;
    }

    std::lock_guard<std::mutex> lock(*mutexShared);

    GL_CHECK();

    _view->render(_viewport);

    if (auto wnd = _view->window()) {
        wnd->resetOpenGLState();
    }

    GL_CHECK();
}

}