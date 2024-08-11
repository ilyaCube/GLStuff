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
#include "Camera.hpp"

namespace Models {

class Surface3D : public Library::Qml::OpenGLPaintedItem
{
    Q_OBJECT
public:
    static constexpr float CAMERA_MOVE_SPEED = 0.1f;
    static constexpr float CAMERA_MOVE_SENSIVITY = 0.1f;

    Surface3D(QQuickItem* parent = nullptr);

    void render(const QSizeF& viewport) const override;
    void keyPressEvent(QKeyEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    bool eventFilter(QObject* object, QEvent* event) override;

    Library::GL::Texture uploadImage(const Library::Imaging::Image& image);
    bool hasUploadedImage() const;

    static QTransform viewPortMatrix(const QRectF& rect, const QSizeF& viewport, double marginPt = 0.0);
    static QTransform screenMatrix(const QSizeF& viewport);

    READONLY_PROPERTY(hasUploadedImage, hasUploadedImage);

public slots:
    void loadImageUrl(const QUrl& imageUrl);

private:
    // QTimer _timer;
    SurfaceDetails::Camera camera;
    float lastX, lastY;
    QMatrix4x4 _projection;
    Library::GL::Texture _texture;
    Library::GL::TexturePool _texPool8C3;
    const Library::GL::ContextAwareWrapper<SurfaceDetails::TextureRenderer> _renderer;

    QVector3D cameraPos{0.0f, 0.0f, 3.0f};
    QVector3D cameraFront{0.0f, 0.0f, -1.0f};
    QVector3D cameraUp{0.0f, 1.0f, 0.0f};

    void setTexture(const Library::GL::Texture& newTexture);
};

}