#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
    Camera();
    Camera(const glm::vec3& pos, float yaw, float pitch, float fov);

    void setPosition(const glm::vec3& pos);
    
    // move
    void move(const glm::vec3& direction);
    void moveForward(float distance);
    void moveRight(float distance);

    // rotation
    void rotate(float yaw, float pitch);
    void rotateY(float yaw);
    void rotateX(float pitch);
    
    // getters
    glm::vec3 getPosition() const { return position_; }
    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;

private:
    glm::vec3 getFrontVector() const;
    glm::vec3 getRightVector() const;

    glm::vec3 position_;
    float yaw_;
    float pitch_;
    float fov_;
};