#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
    enum class Type { Perspective, Isometric };

    Camera();
    Camera(const glm::vec3& pos, float yaw, float pitch, float fov);

    inline void setPosition(const glm::vec3& pos);
    inline void setType(Type type);
    
    // move
    void move(const glm::vec3& direction);
    inline void moveForward(float distance);
    inline void moveRight(float distance);

    // rotation
    void rotate(float yaw, float pitch);
    void rotateY(float yaw);
    void rotateX(float pitch);

    void zoom(float factor);
    
    // getters
    Type getType() const { return type_; }
    glm::vec3 getPosition() const { return position_; }
    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;

private:
    glm::vec3 getFrontVector() const;
    glm::vec3 getRightVector() const;

    Type type_;
    glm::vec3 position_;

    // for perspective type
    float yaw_;
    float pitch_;
    float fov_;

    // for isometric type
    float orthoScale_ = 7.5f;
};

#include <Core/Camera.inl>