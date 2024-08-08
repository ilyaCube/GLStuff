#include "Surface3D.hpp"

#include <QDir>
#include <QDebug>
#include <QTransform>
#include <QDateTime>

#include "GL/GL.hpp"
#include "GL/Helpers.hpp"
#include "GL/GPULoaders.hpp"
#include "Imaging/Helpers.hpp"

#include "AppContext.hpp"

namespace Models {

Surface3D::Surface3D(QQuickItem* parent)
    : OpenGLPaintedItem(parent),
    _texPool8C3(Library::GL::createTexturePool(GL_RGB8)),
    _renderer(0)
{
    _timer.setTimerType(Qt::TimerType::PreciseTimer);
    _timer.setInterval(10);
    _timer.callOnTimeout([this]{
        QMatrix4x4 newTransform;
        newTransform.translate(QVector3D(0.0, 0.0, -5.0));
        newTransform.rotate((QDateTime::currentDateTime().currentMSecsSinceEpoch() / 10) % 360, QVector3D(1.0, 1.0, 1.0));
        {
            std::lock_guard<std::mutex> _lock(*_paintedItemMutex);
            _modelMatrix = newTransform;
        }
        update();
    });
    _timer.start();

    // Use perspective projection
    _projection.perspective(30.0f, 1024.0f / 768.0f, 1.0f, 100);

    QMatrix4x4 view1;
    view1.translate(QVector3D(-4.0, 0.0, -8.0));
    _viewMatrixes.append(view1);

    QMatrix4x4 view2;
    view2.translate(QVector3D(1, 0.0, -3.0));
    _viewMatrixes.append(view2);

    QMatrix4x4 view3;
    view3.translate(QVector3D(2.0, 2, -6.0));
    _viewMatrixes.append(view3);
}

void Surface3D::render(const QSizeF& viewport) const
{
    auto f = Library::GL::functions();
    Library::GL::clearDepthBuffer();
    Library::GL::fillColorBuffer();
    if(_texture == nullptr) {
        return;
    }

    for(const auto& view : _viewMatrixes) {
        _renderer->render(
            _texture,
            _modelMatrix,
            view,
            _projection,
            GL_LINEAR_MIPMAP_NEAREST,
            GL_NEAREST,
            GL_CLAMP_TO_BORDER
        );
    }

    Library::GL::functions()->glFinish();
}

void Surface3D::uploadImage(const Library::Imaging::Image& image)
{
    Library::GL::Texture newTexture;

    if(!image.isEmpty()) {
        AppContext::ptr()->glContext->invokeSync(
            [this, &newTexture, &image] {
                newTexture = Library::GL::uploadImagePooled(image, _texPool8C3, GL_BGR, GL_UNSIGNED_BYTE);
                newTexture->assureMipmapABR();
            }
        );
    }

    setTexture(newTexture);
}

void Surface3D::loadImageUrl(const QUrl& imageUrl)
{
    auto image = imageUrl.isValid() ? Library::Imaging::load(imageUrl.path()) : Library::Imaging::Image();
    uploadImage(image);
}

void Surface3D::setTexture(const Library::GL::Texture& newTexture)
{
    auto mutexPtr = _paintedItemMutex;
    if(mutexPtr == nullptr) {
        return;
    }
    {
        std::lock_guard<std::mutex> _lock(*mutexPtr);
        _texture = newTexture;
    }
    AppContext::ptr()->glContext->invokeSync(
        [this] {
            Library::GL::functions()->glFinish();
        }
    );
    update();
    PROPERTY_CHANGED(hasUploadedImage, hasUploadedImage());
}

bool Surface3D::hasUploadedImage() const
{
    return _texture != nullptr;
}

QTransform Surface3D::viewPortMatrix(const QRectF& rect, const QSizeF& viewport, double marginPt)
{
    if (rect.isEmpty()) {
        return {};
    }

    float z = std::min((viewport.width() - 2.0 * marginPt) / rect.width(), (viewport.height() - 2.0 * marginPt) / rect.height());
    if (!std::isfinite(z)) {
        z = 1.0;
    }

    float dx = -z * (0.5 * rect.width() + rect.x());
    float dy = -z * (0.5 * rect.height() + rect.y());

    return {
        z, 0.0,
        0.0, z,
        -z * (0.5 * rect.width() + rect.x()),
        -z * (0.5 * rect.height() + rect.y())
    };
}

QTransform Surface3D::screenMatrix(const QSizeF& viewport)
{
    return QTransform::fromScale(2.0 / viewport.width(), -2.0 / viewport.height());
}

}