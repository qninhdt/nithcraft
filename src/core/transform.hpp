#pragma once

namespace nith
{
    class Transform
    {
    public:

        Transform(const vec3& position, const vec3& rotation, const vec3& scale);

        vec3 getPosition() const;
        void setPosition(const vec3& position);
        vec3 move(const vec3& delta);

        vec3 getRotation() const;
        void setRotation(const vec3& rotation);
        vec3 rotate(const vec3& delta);

        vec3 getDirection() const;
        void setDirection(const vec3& direction);

    private:
        vec3 m_position;
        vec3 m_rotation;
        vec3 m_scale;
    };
}