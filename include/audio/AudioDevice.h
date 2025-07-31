#pragma once

#include "core/Base.h"

#include <miniaudio.h>

namespace sal
{
    class AudioDevice
    {
    public:
        void Init();
        void Shutdown();
    private:
        friend class Sound;
        //friend class Music;

        ma_engine m_engine = {};
    };
}