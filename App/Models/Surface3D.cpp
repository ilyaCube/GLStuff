#include "Surface3D.hpp"

#include <QDir>
#include <QDebug>
#include <QTransform>

#include "GL/GL.hpp"
#include "GL/Helpers.hpp"
#include "GL/GPULoaders.hpp"
#include "Imaging/Helpers.hpp"

#include "AppContext.hpp"

namespace Models {

Surface3D::Surface3D(QQuickItem* parent)
    : OpenGLPaintedItem(parent),
    _texPool8C3(Library::GL::createTexturePool(GL_RGB8))
{
}

void Surface3D::render(const QSizeF& viewport) const
{
    Library::GL::fillColorBuffer();
    if(_texture == nullptr) {
        return;
    }

    float texW = _texture->width;
    float texH = _texture->height;
    QPolygonF poly{{QPointF(0.0, 0.0), QPointF(texW, 0.0), QPointF(texW, texH), QPointF(0.0, texH)}};

    auto localMatrix = getLocalMatrix(poly.boundingRect(), viewport);
    auto screenMatrix = getScreenMatrix(viewport);
    auto viewMatrix = localMatrix * screenMatrix;

    _renderer->render(_texture, poly, viewMatrix, GL_LINEAR_MIPMAP_NEAREST, GL_NEAREST, GL_CLAMP_TO_BORDER);

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

QTransform Surface3D::getLocalMatrix(const QRectF& rect, const QSizeF& viewport, double marginPt)
{
    if (rect.isEmpty()) {
        return {};
    }

    auto z = std::min((viewport.width() - 2.0 * marginPt) / rect.width(), (viewport.height() - 2.0 * marginPt) / rect.height());
    if (!std::isfinite(z)) {
        z = 1.0;
    }

    return {
        z, 0.0,
        0.0, z,
        -z * (0.5 * rect.width() + rect.x()),
        -z * (0.5 * rect.height() + rect.y())
    };
}

QTransform Surface3D::getScreenMatrix(const QSizeF& viewport)
{
    return QTransform::fromScale(2.0 / viewport.width(), -2.0 / viewport.height());
}

}