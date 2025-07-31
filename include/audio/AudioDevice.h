#pragma once

#include <miniaudio.h>

namespace sal
{
    class AudioDevice
    {
    public:
        void Init();
        void Shutdown();
    private:
        ma_engine m_engine = {};
    };
}