#pragma once

namespace sal::Window {

    void Init(int width, int height, const char* title);
    void Shutdown();
    void SwapBuffers();
    bool Running();

}