#include <Scene/Components/TransformComponent.hpp>

TransformComponent::TransformComponent() : position(0.0f), rotation(0.0f), scale(1.0f)
{
}
TransformComponent::TransformComponent(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& sca) 
    : position(pos), rotation(rot), scale(sca)
{
}
TransformComponent::TransformComponent(const TransformComponent& other)
{
    rotation = other.rotation;
    position = other.position;
    scale = other.scale;
    modelMatrix = other.modelMatrix;
}

void TransformComponent::update([[maybe_unused]] float deltaTime)
{
    glm::mat4 mat = glm::mat4(1.0f);

    mat = glm::translate(mat, position);

    mat = glm::rotate(mat, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    mat = glm::rotate(mat, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    mat = glm::rotate(mat, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    mat = glm::scale(mat, scale);

    modelMatrix = mat;
}

TransformComponent& TransformComponent::operator=(const TransformComponent& other)
{
    // so we can't do something like transformComp = transformComp;
    if (this != &other)
    {
        rotation = other.rotation;
        position = other.position;
        scale = other.scale;
        modelMatrix = other.modelMatrix;
    }
            
    return *this;
}

bool TransformComponent::operator==(const TransformComponent& other) const
{
    return position == other.position &&
           rotation == other.rotation && 
           scale == other.scale;
}

bool TransformComponent::operator!=(const TransformComponent& other) const
{
    return !(*this == other);
}