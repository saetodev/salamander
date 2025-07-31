#include "audio/AudioDevice.h"
#include "audio/Sound.h"

namespace sal
{

    void AudioDevice::Init()
    {
        ma_result result = ma_engine_init(NULL, &m_engine);

        if (result != MA_SUCCESS)
        {
            std::cout << "MA engine init failed" << std::endl;
            std::exit(-1);
        }
    }

    void AudioDevice::Shutdown()
    {
        ma_engine_uninit(&m_engine);
    }

}