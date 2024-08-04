#pragma once

#include <QPainter>
#include <QOpenGLPaintDevice>
#include <QtQuick/QQuickFramebufferObject>
#include "OpenGLPaintedItem.hpp"

namespace Library::Qml {

class Renderer : public QQuickFramebufferObject::Renderer
{
public:
    explicit Renderer(const OpenGLPaintedItem *view);

protected:
    const OpenGLPaintedItem *_view;
    const std::weak_ptr<std::mutex> _weakMutex;
    QSizeF _viewport;

    QOpenGLFramebufferObject *createFramebufferObject(const QSize& size_px) override;
    void render() override;
};

}