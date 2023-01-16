#pragma once

#include "core/ecs/hierarchical_archetype.hpp"

namespace nith
{
    struct SceneComponent
    {
    };

    struct TransformComponent
    {
        vec3 position;
        vec3 scale;
        vec3 rotation;
        mat4 localMatrix;
        mat4 worldMatrix;
        bool isDirty;

        void onAttach()
        {
            position = {0, 0, 0};
            scale = {1, 1, 1};
            rotation = {0, 0, 0};
            isDirty = true;
        }
    };

    using SceneArchetype = ecs::HierarchicalArchetype<
        SceneComponent,
        TransformComponent>;

    class SceneGraph
    {
    public:
    private:
        SceneArchetype sceneArchetype;
    };
} // namespace nith
