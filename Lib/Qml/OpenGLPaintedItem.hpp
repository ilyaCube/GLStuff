#pragma once

#include <mutex>
#include <QtQuick/QQuickFramebufferObject>

namespace Library::Qml {

class OpenGLPaintedItem : public QQuickFramebufferObject
{
public:
    explicit OpenGLPaintedItem(QQuickItem *parent = nullptr);
    ~OpenGLPaintedItem() override;

    virtual void render(const QSizeF& viewport) const = 0;

    QQuickFramebufferObject::Renderer *createRenderer() const override;
    virtual QOpenGLFramebufferObject *createFramebufferObject(const QSize& size_px) const;
    std::weak_ptr<std::mutex> weakMutex() const;

protected:
    const std::shared_ptr<std::mutex> _paintedItemMutex;
};

}