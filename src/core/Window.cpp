#include "core/Input.h"
#include "core/Window.h"

#include <assert.h>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace sal {

    static GLFWwindow* s_window = NULL;

    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        bool isDown  = (action == GLFW_PRESS) || (action == GLFW_REPEAT);
        bool wasDown = (action == GLFW_RELEASE) || (action == GLFW_REPEAT);
        
        Input::Get().SetKey(key, isDown, wasDown);
    }

    void Window::Init(int width, int height, const char* title) {
        assert(!s_window);

        if (!glfwInit()) {
            std::cout << "GLFW init failed" << std::endl;
            std::exit(-1);
        }

        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);

        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        s_window = glfwCreateWindow(width, height, title, NULL, NULL);

        if (!s_window) {
            std::cout << "GLFW create window failed" << std::endl;
            std::exit(-1);
        }

        glfwSetKeyCallback(s_window, KeyCallback);

        glfwMakeContextCurrent(s_window);

        if (!gladLoadGLES2Loader((GLADloadproc)glfwGetProcAddress)) {
            std::cout << "GLAD GLES2 loader failed" << std::endl;
            std::exit(-1);
        }
    }

    void Window::Shutdown() {
        assert(s_window);

        glfwDestroyWindow(s_window);
        glfwTerminate();
    }

    void Window::SwapBuffers() {
        //TODO: should this be here?
        Input::Get().Reset();

        glfwSwapBuffers(s_window);
        glfwPollEvents();
    }

    bool Window::Running() {
        return !glfwWindowShouldClose(s_window);
    }
    
}