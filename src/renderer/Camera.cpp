#include "renderer/Camera.h"

namespace sal {

    Camera::Camera(float left, float right, float bottom, float top) {
        m_projectionMatrix = glm::ortho(left, right, bottom, top);
        m_viewMatrix       = glm::mat4(1.0f);
    }

    void Camera::RecalculateViewMatrix() {
        glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f));
        glm::mat4 rotate    = glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 transform = translate * rotate;
        m_viewMatrix        = glm::inverse(transform);
    }

}