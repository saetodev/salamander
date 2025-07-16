#include "Window.h"

#include <assert.h>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace sal {

    static GLFWwindow* s_window = NULL;

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
        glfwSwapBuffers(s_window);
        glfwPollEvents();
    }

    bool Window::Running() {
        return !glfwWindowShouldClose(s_window);
    }
    
}