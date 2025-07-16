#pragma once

#include "core/Base.h"

namespace sal {

    class Camera {
    public:
        glm::vec2 position = {};
        float     rotation = 0.0f;

        Camera() = default;
        Camera(float left, float right, float bottom, float top);

        void RecalculateViewMatrix();

        const glm::mat4& ProjectionMatrix() const { return m_projectionMatrix; }
        const glm::mat4& ViewMatrix() const { return m_viewMatrix; }
    private:
        glm::mat4 m_projectionMatrix = {};
        glm::mat4 m_viewMatrix       = {};
    };

}