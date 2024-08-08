#pragma once

#include <QTimer>
#include <QMatrix4x4>
#include <QTransform>
#include <qmatrix4x4.h>
#include <qvector.h>
#include "Qml/OpenGLPaintedItem.hpp"
#include "GL/Texture.hpp"
#include "GL/TexturePool.hpp"
#include "GL/ContextAwareWrapper.hpp"
#include "Macro/Qt.hpp"
#include "TextureRenderer.hpp"

namespace Models {

class Surface3D : public Library::Qml::OpenGLPaintedItem
{
    Q_OBJECT
public:
    Surface3D(QQuickItem* parent = nullptr);

    void render(const QSizeF& viewport) const override;
    void uploadImage(const Library::Imaging::Image& image);
    bool hasUploadedImage() const;

    static QTransform viewPortMatrix(const QRectF& rect, const QSizeF& viewport, double marginPt = 0.0);
    static QTransform screenMatrix(const QSizeF& viewport);

    READONLY_PROPERTY(hasUploadedImage, hasUploadedImage);

public slots:
    void loadImageUrl(const QUrl& imageUrl);

private:
    QTimer _timer;
    QMatrix4x4 _projection;
    QMatrix4x4 _modelMatrix;
    QVector<QMatrix4x4> _viewMatrixes;
    Library::GL::Texture _texture;
    Library::GL::TexturePool _texPool8C3;
    const Library::GL::ContextAwareWrapper<SurfaceDetails::TextureRenderer> _renderer;

    void setTexture(const Library::GL::Texture& newTexture);
};

}