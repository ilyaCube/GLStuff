#pragma once

#include "Qml/OpenGLPaintedItem.hpp"
#include "GL/Texture.hpp"
#include "GL/TexturePool.hpp"
#include "GL/ContextAwareWrapper.hpp"
#include "Macro/Qt.hpp"
#include "Renderer.hpp"

namespace Models {

class Surface3D : public Library::Qml::OpenGLPaintedItem
{
    Q_OBJECT
public:
    Surface3D(QQuickItem* parent = nullptr);

    void render(const QSizeF& viewport) const override;
    void uploadImage(const Library::Imaging::Image& image);
    bool hasUploadedImage() const;

    static QTransform getLocalMatrix(const QRectF& rect, const QSizeF& viewport, double marginPt = 0.0);
    static QTransform getScreenMatrix(const QSizeF& viewport);

    READONLY_PROPERTY(hasUploadedImage, hasUploadedImage);

public slots:
    void loadImageUrl(const QUrl& imageUrl);

private:
    Library::GL::Texture _texture;
    Library::GL::TexturePool _texPool8C3;
    const Library::GL::ContextAwareWrapper<SurfaceDetails::Renderer> _renderer;

    void setTexture(const Library::GL::Texture& newTexture);
};

}