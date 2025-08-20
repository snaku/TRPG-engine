#include <Core/Camera.hpp>

Camera::Camera() : position_(0.0f), yaw_(-90.0f), pitch_(0), fov_(45.0f)
{
}
Camera::Camera(const glm::vec3& pos, float yaw, float pitch, float fov)
    : position_(pos), yaw_(yaw), pitch_(pitch), fov_(fov)
{
}

void Camera::move(const glm::vec3& direction)
{
    if (type_ == Type::Perspective)
    {
        glm::vec3 front = getFrontVector();
        glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));

        position_ += front * direction.z;
        position_ += right * direction.x;
        position_ += glm::vec3(0.0f, 1.0f, 0.0f) * direction.y;
    }
    else 
    {
        glm::vec3 isoFront = glm::normalize(glm::vec3(1, 0, 1));
        glm::vec3 isoRight = glm::normalize(glm::vec3(1, 0, -1));
        position_ += isoFront * direction.z + isoRight * direction.x + glm::vec3(0, 1, 0) * direction.y;
    }
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

void Camera::zoom(float factor)
{
    if (type_ == Type::Perspective)
        fov_ *= factor;
    else
        orthoScale_ *= factor;
}

glm::vec3 Camera::getFrontVector() const
{
    if (type_ == Type::Perspective)
    {
        glm::vec3 front;

        front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
        front.y = sin(glm::radians(pitch_));
        front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));

        return glm::normalize(front);
    }    
    else 
    {
        return glm::normalize(glm::vec3(1.0f, -1.0f, 1.0f));
    }   
}

glm::vec3 Camera::getRightVector() const
{
    glm::vec3 front = getFrontVector();

    return glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));
}

glm::mat4 Camera::getProjectionMatrix() const
{
    if (type_ == Type::Perspective)
    {
        glm::mat4 projection = glm::perspective(glm::radians(fov_), 16.0f/9.0f, 0.1f, 100.0f);
        projection[1][1] *= -1.0f; // invert the Y axis
        
        return projection;
    }
    else
    {
        float orthoScale = orthoScale_;
        float aspect = 16.0f / 9.0f;
        glm::mat4 projection = glm::ortho(-orthoScale * aspect, orthoScale * aspect, -orthoScale, orthoScale, -100.0f, 100.0f);
        projection[1][1] *= -1.0f; // invert the Y axis
        
        return projection;
    }
}

glm::mat4 Camera::getViewMatrix() const 
{ 
    if (type_ == Type::Perspective)
    {
        return glm::lookAt(position_, position_ + getFrontVector(), glm::vec3(0.0f, 1.0f, 0.0f)); 
    }
    else
    {
        glm::vec3 isoPosition = glm::vec3(position_);
        glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

        return glm::lookAt(isoPosition, target, up);
    }
}