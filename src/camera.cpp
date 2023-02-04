#include "camera.hpp"
#include "camera.hpp"

namespace nith
{
    Camera::Camera(const f32& aspect, const f32& fov, const f32& near, const f32& far) :
        m_aspect(aspect),
        m_fov(fov),
        m_near(near),
        m_far(far),
        m_zoom(1),
        m_transform({ 0, 0, 0 }, { 0, 0, 0 }, { 1, 1, 1 }),
        m_up(YAxis)
    {
        updateView();
        updateProjection();
    }

    Transform& Camera::getTransform()
    {
        return m_transform;
    }

    const mat4& Camera::getView() const
    {
        return m_view;
    }

    const mat4& Camera::getProjectionView() const
    {
        return m_projectionView;
    }

    vec3 Camera::getUp() const
    {
        return m_up;
    }

    vec3 Camera::getRight() const
    {
        return m_right;
    }

    vec3 Camera::getForward() const
    {
        return m_forward;
    }

    void Camera::updateView()
    {
        vec3 direction = m_transform.getDirection();
        m_view = glm::lookAt(m_transform.getPosition(), m_transform.getPosition() + direction, m_up);
        m_right = glm::normalize(glm::cross(direction, m_up));
        m_forward = glm::normalize(glm::cross(m_up, m_right));
        updateProjectionView();
    }

    void Camera::updateProjection()
    {
        m_projection = glm::perspective(m_fov, m_aspect, m_near, m_far);
        updateProjectionView();
    }

    void Camera::updateProjectionView()
    {
        m_projectionView = m_projection * m_view;
    }
}