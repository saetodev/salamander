#include "core/Input.h"

namespace sal {

    static Input s_input;

    void Input::Reset() {
        m_keysPressed.fill(false);
        m_keysReleased.fill(false);
    }

    void Input::SetKey(int key, bool isDown, bool wasDown) {
        if (key < 0 || key >= KEY_BUFFER_SIZE) {
            return;
        }

        m_keysDown[key]     = isDown;
        m_keysPressed[key]  = isDown && !wasDown;
        m_keysReleased[key] = !isDown && wasDown; 
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

    Input& Input::Get() {
        return s_input;
    }

}