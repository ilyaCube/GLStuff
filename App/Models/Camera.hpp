#pragma once

#include <QMatrix4x4>

namespace Models::SurfaceDetails {

class Camera
{
public:
    struct CameraContext
    {
        QVector3D cameraPos{0.0f, 0.0f, 3.0f};
        QVector3D cameraFront{0.0f, 0.0f, -1.0f};
        QVector3D cameraUp{0.0f, 1.0f, 0.0f};

        float yaw = 0.0f;
        float pitch = 0.0f;
    };

    enum CameraDirections : uint16_t
    {
        NONE        = 0,
        UP          = 1 << 0,
        DOWN        = 1 << 1,
        RIGHT       = 1 << 2,
        LEFT        = 1 << 3,
        FORWARD     = 1 << 4,
        BACKWARD    = 1 << 5
    };

    Camera() = default;
    QMatrix4x4 currentLookAt() const;
    void reset();

    void move(CameraDirections maskDirections, float speed);

    void moveAngle(float offsetYaw, float offsetPitch, float sensitivity);
    void moveForward(float speed);
    void moveBackward(float speed);
    void moveUp(float speed);
    void moveDown(float speed);
    void moveLeft(float speed);
    void moveRight(float speed);

private:
    mutable std::mutex _mut;
    CameraContext ctx;

    void moveForwardImpl(float speed);
    void moveBackwardImpl(float speed);
    void moveUpImpl(float speed);
    void moveDownImpl(float speed);
    void moveLeftImpl(float speed);
    void moveRightImpl(float speed);
};

}