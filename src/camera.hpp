#pragma once

#include "core/transform.hpp"

namespace nith
{
    class Camera
    {
    public:
        static constexpr vec3 XAxis = { 1, 0, 0 };
        static constexpr vec3 YAxis = { 0, 1, 0 };
        static constexpr vec3 ZAxis = { 0, 0, 1 };

        Camera(const f32& aspect, const f32& fov, const f32& near, const f32& far);

        Transform& getTransform();

        const mat4& getView() const;
        const mat4& getProjectionView() const;

        vec3 getUp() const;
        vec3 getRight() const;
        vec3 getForward() const;

        void updateView();
        void updateProjection();

    private:
        void updateProjectionView();

        f32 m_aspect;
        f32 m_fov;
        f32 m_near;
        f32 m_far;
        f32 m_zoom;
        vec3 m_direction;
        vec3 m_forward;
        vec3 m_right;
        vec3 m_up;
        Transform m_transform;
        mat4 m_view;
        mat4 m_projection;
        mat4 m_projectionView;
    };
}