#pragma once

#include "core/Base.h"

#include <miniaudio.h>

namespace sal
{
    class AudioDevice;

    enum class EAudioFormat
    {
        NONE,
        U8,
        S16,
        S24,
        S32,
        F32,
    };

    struct SoundDesc
    {
        EAudioFormat format;
        uint32_t     channels;
        uint32_t     sampleRate;
        size_t       frameCount;
        void*        data;
    };

    class Sound
    {
    public:
        Sound(AudioDevice& device, const SoundDesc& desc);
        ~Sound();

        void Play();

        static Ref<Sound> Load(AudioDevice& device, std::string_view filename);
    private:
        SoundDesc       m_desc   = {};
        ma_audio_buffer m_buffer = {};
        ma_sound        m_sound  = {};
    };
}