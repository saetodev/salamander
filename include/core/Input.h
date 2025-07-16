#pragma once

#include "core/Base.h"

namespace sal {

    class Input {
    public:
        void Reset();
        void SetKey(int key, bool isDown, bool wasDown);    

        bool KeyDown(int key) const;
        bool KeyPressed(int key) const;
        bool KeyReleased(int key) const;

        static Input& Get();
    private:
        static constexpr int KEY_BUFFER_SIZE = 256;

        std::array<bool, KEY_BUFFER_SIZE> m_keysDown     = {};
        std::array<bool, KEY_BUFFER_SIZE> m_keysPressed  = {};
        std::array<bool, KEY_BUFFER_SIZE> m_keysReleased = {};
    };

}