#include "core/Input.h"

namespace sal {

    void Input::Reset() {
        m_keysPressed.fill(false);
        m_keysReleased.fill(false);
    }

    void Input::SetMousePosition(const glm::vec2& position) {
        m_mousePosition = position;
    }

    void Input::SetKey(int key, bool isDown, bool wasDown) {
        if (key < 0 || key >= KEY_BUFFER_SIZE) {
            return;
        }

        m_keysDown[key]     = isDown;
        m_keysPressed[key]  = isDown && !wasDown;
        m_keysReleased[key] = !isDown && wasDown; 
    } 

    void Input::SetMouseButton(int button, bool isDown, bool wasDown) {
        if (button < 0 || button >= BUTTON_BUFFER_SIZE) {
            return;
        }

        m_buttonsDown[button]     = isDown;
        m_buttonsPressed[button]  = isDown && !wasDown;
        m_buttonsReleased[button] = !isDown && wasDown; 
    }

    bool Input::KeyDown(int key) const {
        if (key < 0 || key >= KEY_BUFFER_SIZE) {
            return false;
        }

        return m_keysDown[key];
    }

    bool Input::KeyPressed(int key) const {
        if (key < 0 || key >= KEY_BUFFER_SIZE) {
            return false;
        }

        return m_keysPressed[key];
    }

    bool Input::KeyReleased(int key) const {
        if (key < 0 || key >= KEY_BUFFER_SIZE) {
            return false;
        }

        return m_keysReleased[key];
    }

    bool Input::MouseButtonDown(int button) const {
        if (button < 0 || button >= BUTTON_BUFFER_SIZE) {
            return false;
        }

        return m_buttonsDown[button];
    }

    bool Input::MouseButtonPressed(int button) const {
        if (button < 0 || button >= BUTTON_BUFFER_SIZE) {
            return false;
        }

        return m_buttonsPressed[button];
    }

    bool Input::MouseButtonReleased(int button) const {
        if (button < 0 || button >= BUTTON_BUFFER_SIZE) {
            return false;
        }

        return m_buttonsReleased[button];
    }
    
}