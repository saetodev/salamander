#include "audio/AudioDevice.h"
#include "audio/Sound.h"

namespace sal
{

    static ma_format GetMAAudioFormat(EAudioFormat format)
    {
        switch (format)
        {
            case EAudioFormat::U8: return ma_format_u8;
            case EAudioFormat::S16: return ma_format_s16;
            case EAudioFormat::S24: return ma_format_s24;
            case EAudioFormat::S32: return ma_format_s32;
            case EAudioFormat::F32: return ma_format_f32;
        }

        ASSERT(false);
        return ma_format_unknown;
    }

    static EAudioFormat GetAudioFormat(ma_format format)
    {
        switch (format)
        {
            case ma_format_u8: return EAudioFormat::U8;
            case ma_format_s16: return EAudioFormat::S16;
            case ma_format_s24: return EAudioFormat::S24;
            case ma_format_s32: return EAudioFormat::S32;
            case ma_format_f32: return EAudioFormat::F32;
        }

        ASSERT(false);
        return EAudioFormat::NONE;
    }

    Sound::Sound(AudioDevice& device, const SoundDesc& desc)
    {
        ma_result result = {};

        ma_audio_buffer_config bufferConfig = ma_audio_buffer_config_init(GetMAAudioFormat(desc.format), desc.channels, desc.frameCount, desc.data, NULL);

        result = ma_audio_buffer_init(&bufferConfig, &m_buffer);

        if (result != MA_SUCCESS)
        {
            //TODO: handle this properly
            ASSERT(false);
        }

        result = ma_sound_init_from_data_source(&device.m_engine, &m_buffer, MA_SOUND_FLAG_DECODE, NULL, &m_sound);

        if (result != MA_SUCCESS)
        {
            //TODO: handle this properly
            ASSERT(false);
        }

        m_desc = desc;
    }

    Sound::~Sound()
    {
        ma_sound_uninit(&m_sound);
        ma_audio_buffer_uninit(&m_buffer);
        delete[] m_desc.data;
    }

    void Sound::Play()
    {
        ma_sound_start(&m_sound);
    }

    Ref<Sound> Sound::Load(AudioDevice& device, std::string_view filename)
    {
        SoundDesc desc = {};

        ma_result result   = {};
        ma_decoder decoder = {};
        ma_format format   = ma_format_unknown;

        result = ma_decoder_init_file(filename.data(), NULL, &decoder);

        if (result != MA_SUCCESS)
        {
            //TODO: handle this properly
            ASSERT(false);
        }

        result = ma_decoder_get_data_format(&decoder, &format, &desc.channels, &desc.sampleRate, NULL, 0);

        if (result != MA_SUCCESS)
        {
            //TODO: handle this properly
            ASSERT(false);
        }

        desc.format = GetAudioFormat(format);
        
        ma_uint64 frameCount = 0;
        result = ma_decoder_get_length_in_pcm_frames(&decoder, &frameCount);
        desc.frameCount = frameCount;

        if (result != MA_SUCCESS)
        {
            //TODO: handle this properly
            ASSERT(false);
        }

        desc.data = new float[desc.frameCount * desc.channels];

        result = ma_decoder_read_pcm_frames(&decoder, desc.data, desc.frameCount, NULL);

        if (result != MA_SUCCESS)
        {
            //TODO: handle this properly
            ASSERT(false);
        }

        Ref<Sound> sound = MakeRef<Sound>(device, desc);

        ma_decoder_uninit(&decoder);

        return sound;
    }

}
