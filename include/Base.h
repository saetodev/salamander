#pragma once

#include <cassert>
#include <memory>
#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace sal {

    template<typename T>
    using Ref = std::shared_ptr<T>;

    template<typename T>
    using Scope = std::unique_ptr<T>;

    template<typename T, typename... Args>
    constexpr Ref<T> MakeRef(Args&&... args) {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    template<typename T, typename... Args>
    constexpr Scope<T> MakeScope(Args&&... args) {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    inline glm::mat4 MakeTransform(const glm::vec2& position, const glm::vec2& size, float rotation) {
        glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f));
        glm::mat4 rotate    = glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 scale     = glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));

        return translate * rotate * scale;
    }

}