#include "Camera.hpp"

#include <QtMath>

namespace Models::SurfaceDetails {

QMatrix4x4 Camera::currentLookAt() const
{
    std::lock_guard<std::mutex> lock(_mut);
    QMatrix4x4 lookAt;
    lookAt.lookAt(ctx.cameraPos, ctx.cameraPos + ctx.cameraFront, ctx.cameraUp);
    return lookAt;
}

void Camera::reset()
{
    std::lock_guard<std::mutex> lock(_mut);
    ctx = {};
}

void Camera::moveAngle(float offsetYaw, float offsetPitch, float sensitivity)
{
    std::lock_guard<std::mutex> lock(_mut);

    offsetYaw *= sensitivity;
    offsetPitch *= sensitivity;

    ctx.yaw += offsetYaw;
    ctx.pitch += offsetPitch;

    ///
    if(ctx.pitch > 89.0f) {
        ctx.pitch = 89.0f;
    }
    if(ctx.pitch < -89.0f) {
        ctx.pitch = -89.0f;
    }
    ///

    auto yawRad = qDegreesToRadians(ctx.yaw);
    auto pitchRad = qDegreesToRadians(ctx.pitch);
    QVector3D direction;
    direction.setX(qCos(yawRad) * qCos(pitchRad));
    direction.setY(qSin(pitchRad));
    direction.setZ(qSin(yawRad) * qCos(pitchRad));
    ctx.cameraFront = direction.normalized();
}

void Camera::move(CameraDirections maskDirections, float speed)
{
    std::lock_guard<std::mutex> lock(_mut);

    if(maskDirections & CameraDirections::UP) {
        moveUpImpl(speed);
    }
    if(maskDirections & CameraDirections::DOWN) {
        moveDownImpl(speed);
    }
    if(maskDirections & CameraDirections::RIGHT) {
        moveRightImpl(speed);
    }
    if(maskDirections & CameraDirections::LEFT) {
        moveLeftImpl(speed);
    }
    if(maskDirections & CameraDirections::FORWARD) {
        moveForwardImpl(speed);
    }
    if(maskDirections & CameraDirections::BACKWARD) {
        moveBackwardImpl(speed);
    }
}

void Camera::moveForward(float speed)
{
    std::lock_guard<std::mutex> lock(_mut);
    moveForwardImpl(speed);
}

void Camera::moveBackward(float speed)
{
    std::lock_guard<std::mutex> lock(_mut);
    moveBackwardImpl(speed);
}

void Camera::moveLeft(float speed)
{
    std::lock_guard<std::mutex> lock(_mut);
    moveLeftImpl(speed);
}

void Camera::moveRight(float speed)
{
    std::lock_guard<std::mutex> lock(_mut);
    moveRightImpl(speed);
}

void Camera::moveUp(float speed)
{

}

void Camera::moveDown(float speed)
{

}

void Camera::moveUpImpl(float speed)
{

}

void Camera::moveDownImpl(float speed)
{

}

void Camera::moveForwardImpl(float speed)
{
    ctx.cameraPos += speed * ctx.cameraFront;
}

void Camera::moveBackwardImpl(float speed)
{
    ctx.cameraPos -= speed * ctx.cameraFront;
}

void Camera::moveLeftImpl(float speed)
{
    auto leftRightMovable = QVector3D::crossProduct(ctx.cameraFront, ctx.cameraUp).normalized();
    ctx.cameraPos -= speed * leftRightMovable;
}

void Camera::moveRightImpl(float speed)
{
    auto leftRightMovable = QVector3D::crossProduct(ctx.cameraFront, ctx.cameraUp).normalized();
    ctx.cameraPos += speed * leftRightMovable;
}

}