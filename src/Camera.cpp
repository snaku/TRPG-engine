#include "Include/Camera.hpp"

Camera::Camera() : position_(0.0f), yaw_(-90.0f), pitch_(0), fov_(45.0f)
{
}
Camera::Camera(const glm::vec3& pos, float yaw, float pitch, float fov)
    : position_(pos), yaw_(yaw), pitch_(pitch), fov_(fov)
{
}

void Camera::setPosition(const glm::vec3& pos)
{
    position_ = pos;
}

void Camera::move(const glm::vec3& direction)
{
    glm::vec3 front = getFrontVector();
    glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));

    position_ += front * direction.z;
    position_ += right * direction.x;
    position_ += glm::vec3(0.0f, 1.0f, 0.0f) * direction.y;
}

void Camera::rotate(float yaw, float pitch)
{
    yaw_ += yaw;
    pitch_ += pitch;

    if (pitch_ > 89.0f) pitch_ = 89.0f;
    if (pitch_ < -89.0f) pitch_ = -89.0f;
}

void Camera::rotateX(float pitch)
{
    pitch_ += pitch;

    if (pitch_ > 89.0f) pitch_ = 89.0f;
    if (pitch_ < -89.0f) pitch_ = -89.0f;
}

void Camera::rotateY(float yaw)
{
    yaw_ += yaw;
}

glm::vec3 Camera::getFrontVector() const
{
    glm::vec3 front;

    front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front.y = sin(glm::radians(pitch_));
    front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));

    return glm::normalize(front);
}

glm::vec3 Camera::getRightVector() const
{
    glm::vec3 front = getFrontVector();

    return glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));
}

glm::mat4 Camera::getProjectionMatrix() const
{
    glm::mat4 projection = glm::perspective(glm::radians(fov_), 16.0f/9.0f, 0.1f, 100.0f);
    
    projection[1][1] *= -1.0f; // invert the Y axis
    
    return projection;
}

glm::mat4 Camera::getViewMatrix() const 
{ 
    return glm::lookAt(position_, position_ + getFrontVector(), glm::vec3(0.0f, 1.0f, 0.0f)); 
}; 

void Camera::moveForward(float distance)
{
    position_ += getFrontVector() * distance;
}

void Camera::moveRight(float distance)
{
    position_ += getRightVector() * distance;
}