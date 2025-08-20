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
    move(glm::vec3(0, 0, distance));
}

inline void Camera::moveRight(float distance)
{
    move(glm::vec3(distance, 0, 0));
}