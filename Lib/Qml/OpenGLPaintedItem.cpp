#include "OpenGLPaintedItem.hpp"

#include <QOpenGLFramebufferObject>
#include "OpenGLRenderer.hpp"

namespace Library::Qml {

OpenGLPaintedItem::OpenGLPaintedItem(QQuickItem *parent) :
    QQuickFramebufferObject(parent),
    _paintedItemMutex(std::make_shared<std::mutex>())
{
    // Assure correct OpenGL coordinates (Qml flips them by default)
    setMirrorVertically(true);
}

OpenGLPaintedItem::~OpenGLPaintedItem()
{
    std::lock_guard<std::mutex> lock(*_paintedItemMutex);
}

QQuickFramebufferObject::Renderer *OpenGLPaintedItem::createRenderer() const
{
    return new Qml::Renderer(this);
}

QOpenGLFramebufferObject *OpenGLPaintedItem::createFramebufferObject(const QSize& size_px) const
{
    return new QOpenGLFramebufferObject(size_px, QOpenGLFramebufferObjectFormat());
}

std::weak_ptr<std::mutex> OpenGLPaintedItem::weakMutex() const
{
    return _paintedItemMutex;
}

}