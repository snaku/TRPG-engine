#include <Core/Camera.hpp>

inline void Camera::setPosition(const glm::vec3& pos)
{
    position_ = pos;
}

inline void Camera::setType(Type type)
{
    type_ = type;
}

inline void Camera::moveForward(float distance)
{
    move({0, 0, distance});
}

inline void Camera::moveRight(float distance)
{
    move({distance, 0, 0});
}

inline void Camera::rotateY(float yaw)
{
    rotate(yaw, 0);
}

inline void Camera::rotateX(float pitch)
{
    rotate(0, pitch);
}