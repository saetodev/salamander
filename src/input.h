#ifndef SAL_INPUT_H
#define SAL_INPUT_H

#include <array>

class Input {
public:
    void clear() {
        m_keys_pressed.fill(false);
        m_keys_released.fill(false);
    }

    void set(int key, bool is_down, bool was_down) {
        if (key < 0 || key >= KEY_BUFFER_SIZE) {
            return;
        }

        m_keys_down[key] = is_down;
        m_keys_pressed[key] = is_down && !was_down;
        m_keys_released[key] = !is_down && was_down;
    }

    bool key_down(int key) {
        if (key < 0 || key >= KEY_BUFFER_SIZE) {
            return false;
        }

        return m_keys_down[key];
    }

    bool key_pressed(int key) {
        if (key < 0 || key >= KEY_BUFFER_SIZE) {
            return false;
        }

        return m_keys_pressed[key];
    }

    bool key_released(int key) {
        if (key < 0 || key >= KEY_BUFFER_SIZE) {
            return false;
        }

        return m_keys_released[key];
    }

private:
    static constexpr int KEY_BUFFER_SIZE = 256;
    
    std::array<bool, KEY_BUFFER_SIZE> m_keys_down = {};
    std::array<bool, KEY_BUFFER_SIZE> m_keys_pressed = {};
    std::array<bool, KEY_BUFFER_SIZE> m_keys_released = {};
};

#endif