#include "core/transform.hpp"

namespace nith
{
    Transform::Transform(const vec3& position, const vec3& rotation, const vec3& scale):
        m_position(position),
        m_rotation(rotation),
        m_scale(scale)
    {
    }

    vec3 Transform::getPosition() const
    {
        return m_position;
    }

    void Transform::setPosition(const vec3& position)
    {
        m_position = position;
    }

    vec3 Transform::move(const vec3& delta)
    {
        return m_position += delta;
    }

    vec3 Transform::getRotation() const
    {
        return m_rotation;
    }

    void Transform::setRotation(const vec3& rotation)
    {
        m_rotation = rotation;
    }

    vec3 Transform::rotate(const vec3& delta)
    {
        setRotation(m_rotation + delta);
        return m_rotation;
    }

    vec3 Transform::getDirection() const
    {
        return {
            sin(m_rotation.y) * cos(m_rotation.x),
            sin(m_rotation.x),
            cos(m_rotation.y) * cos(m_rotation.x),
        };
    }

    void Transform::setDirection(const vec3& direction)
    {
        m_rotation.x = asin(direction.y);
        m_rotation.y = atan2(direction.x, direction.z);
    }
}