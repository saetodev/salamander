#pragma once

#include "core/Base.h"

namespace sal {

    class Input {
    public:
        void Reset();

        void SetMousePosition(const glm::vec2& position);
        void SetKey(int key, bool isDown, bool wasDown);
        void SetMouseButton(int button, bool isDown, bool wasDown);

        glm::vec2 MousePosition() const { return m_mousePosition; }

        bool KeyDown(int key) const;
        bool KeyPressed(int key) const;
        bool KeyReleased(int key) const;

        bool MouseButtonDown(int button) const;
        bool MouseButtonPressed(int button) const;
        bool MouseButtonReleased(int button) const;

        static Input& Get();
    private:
        static constexpr int KEY_BUFFER_SIZE    = 256;
        static constexpr int BUTTON_BUFFER_SIZE = 8;

        glm::vec2 m_mousePosition = {};

        std::array<bool, KEY_BUFFER_SIZE> m_keysDown     = {};
        std::array<bool, KEY_BUFFER_SIZE> m_keysPressed  = {};
        std::array<bool, KEY_BUFFER_SIZE> m_keysReleased = {};

        std::array<bool, BUTTON_BUFFER_SIZE> m_buttonsDown     = {};
        std::array<bool, BUTTON_BUFFER_SIZE> m_buttonsPressed  = {};
        std::array<bool, BUTTON_BUFFER_SIZE> m_buttonsReleased = {};
    };

}