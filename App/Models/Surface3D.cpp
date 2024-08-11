#include "Surface3D.hpp"

#include <QDir>
#include <QDebug>
#include <QTransform>
#include <QDateTime>

#include "GL/GL.hpp"
#include "GL/Helpers.hpp"
#include "GL/GPULoaders.hpp"
#include "Imaging/Helpers.hpp"

#include <QApplication>
#include "AppContext.hpp"

namespace {

QVector<QVector3D> cubes {
    QVector3D(0.0f, 0.0f, -3.5),
    QVector3D(2.0f, 0.0f, -3.5),
    QVector3D(-2.0f, 0.0f, -3.5)
};

QVector<GLfloat> verticesCube{
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

QVector<GLfloat> verticesImage{

0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,

-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
-0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
0.5f, 0.5f, 0.0f, 1.0f, 1.0f
};

}

namespace Models {

Surface3D::Surface3D(QQuickItem* parent)
    : OpenGLPaintedItem(parent),
    _texPool8C3(Library::GL::createTexturePool(GL_RGB8)),
    _renderer(0)
{
    // Use perspective projection
    _projection.perspective(30.0f, 1024.0f / 768.0f, 1.0f, 100);

    QApplication::instance()->installEventFilter(this);
    setAcceptedMouseButtons(Qt::AllButtons);
    setAcceptHoverEvents(true);
    setFocus(true);
}

void Surface3D::render(const QSizeF& viewport) const
{
    auto f = Library::GL::functions();
    Library::GL::clearDepthBuffer();
    Library::GL::fillColorBuffer();
    if(_texture == nullptr) {
        return;
    }

    QMatrix4x4 viewMatrix = camera.currentLookAt();
    for(const auto& cudePos : cubes) {
        QMatrix4x4 newModel;
        newModel.translate(cudePos);
        _renderer->render(
            _texture,
            verticesCube,
            newModel,
            viewMatrix,
            _projection,
            GL_LINEAR_MIPMAP_NEAREST,
            GL_NEAREST,
            GL_CLAMP_TO_BORDER
        );
    }


    Library::GL::functions()->glFinish();
}

void Surface3D::keyPressEvent(QKeyEvent *event)
{
    int directions = SurfaceDetails::Camera::CameraDirections::NONE;
    if (event->key() == Qt::Key::Key_W) {
        directions |= SurfaceDetails::Camera::CameraDirections::FORWARD;
    }
    if (event->key() == Qt::Key::Key_S) {
        directions |= SurfaceDetails::Camera::CameraDirections::BACKWARD;
    }
    if (event->key() == Qt::Key::Key_A) {
        directions |= SurfaceDetails::Camera::CameraDirections::LEFT;
    }
    if (event->key() == Qt::Key::Key_D) {
        directions |= SurfaceDetails::Camera::CameraDirections::RIGHT;
    }

    if(directions != SurfaceDetails::Camera::CameraDirections::NONE) {
        camera.move(SurfaceDetails::Camera::CameraDirections(directions), CAMERA_MOVE_SPEED);
        forceActiveFocus();
        update();
    }
    event->ignore();
}

void Surface3D::mouseMoveEvent(QMouseEvent *event)
{
    // tmp
    static bool first = true;
    if(first) {
        lastX = event->x();
        lastY = event->y();
        first = false;
        return;
    }

    auto xpos = event->x();
    auto ypos = event->y();
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = event->x();
    lastY = event->y();
    camera.moveAngle(xoffset, yoffset, 0.1f);
    update();
}

bool Surface3D::eventFilter(QObject* object, QEvent* event)
{
    if(event->type() == QEvent::MouseMove) {
        QMouseEvent* mev=static_cast<QMouseEvent*>(event);
        mouseMoveEvent(mev);
    }
    return false;
}

Library::GL::Texture Surface3D::uploadImage(const Library::Imaging::Image& image)
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

    return newTexture;
}

void Surface3D::loadImageUrl(const QUrl& imageUrl)
{
    auto image = imageUrl.isValid() ? Library::Imaging::load(imageUrl.path()) : Library::Imaging::Image();
    auto tex = uploadImage(image);
    setTexture(tex);
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