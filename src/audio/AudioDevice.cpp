#include "audio/AudioDevice.h"
#include "audio/Sound.h"

namespace sal
{

    void AudioDevice::Init()
    {
        ma_result result = {};

        ma_backend backends[] = {
            ma_backend_wasapi,
            ma_backend_alsa,
            ma_backend_null,
        };

        result = ma_context_init(backends, 3, NULL, &m_context);

        if (result != MA_SUCCESS)
        {
            std::cout << "MA context init failed" << std::endl;
            std::exit(-1);
        }

        result = ma_engine_init(NULL, &m_engine);

        if (result != MA_SUCCESS)
        {
            std::cout << "MA engine init failed" << std::endl;
            std::exit(-1);
        }
    }

    void AudioDevice::Shutdown()
    {
        ma_engine_stop(&m_engine);
        ma_engine_uninit(&m_engine);
        ma_context_uninit(&m_context);
    }

}