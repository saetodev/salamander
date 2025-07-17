#include "core/Input.h"
#include "core/Window.h"

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace sal {

    static void MousePosCallback(GLFWwindow* handle, double xpos, double ypos) {
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(handle));

        float x = xpos / window->XScale();
        float y = ypos / window->YScale();

        Input::Get().SetMousePosition({ x, y });
    }

    static void KeyCallback(GLFWwindow* handle, int key, int scancode, int action, int mods) {
        bool isDown  = (action == GLFW_PRESS) || (action == GLFW_REPEAT);
        bool wasDown = (action == GLFW_RELEASE) || (action == GLFW_REPEAT);
        
        Input::Get().SetKey(key, isDown, wasDown);
    }

    static void MouseButtonCallback(GLFWwindow* handle, int button, int action, int mods) {
        bool isDown  = (action == GLFW_PRESS) || (action == GLFW_REPEAT);
        bool wasDown = (action == GLFW_RELEASE) || (action == GLFW_REPEAT);

        Input::Get().SetMouseButton(button, isDown, wasDown);
    }

    Window::Window(int width, int height, const char* title) {
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

        m_handle = glfwCreateWindow(width, height, title, NULL, NULL);

        if (!m_handle) {
            std::cout << "GLFW create window failed" << std::endl;
            std::exit(-1);
        }

        glfwSetWindowUserPointer(m_handle, this);

        glfwSetCursorPosCallback(m_handle, MousePosCallback);
        glfwSetKeyCallback(m_handle, KeyCallback);
        glfwSetMouseButtonCallback(m_handle, MouseButtonCallback);

        glfwMakeContextCurrent(m_handle);

        if (!gladLoadGLES2Loader((GLADloadproc)glfwGetProcAddress)) {
            std::cout << "GLAD GLES2 loader failed" << std::endl;
            std::exit(-1);
        }

        m_width  = width;
        m_height = height;

        int realWidth  = 0;
        int realHeight = 0;

        glfwGetWindowSize(m_handle, &realWidth, &realHeight);

        m_xscale = (float)realWidth / (float)m_width;
        m_yscale = (float)realHeight / (float)m_height;
    }

    Window::~Window() {
        glfwDestroyWindow(m_handle);
        glfwTerminate();
    }

    void Window::SwapBuffers() {
        //TODO: should this be here?
        Input::Get().Reset();

        glfwSwapBuffers(m_handle);
        glfwPollEvents();

        float nowTime = (float)glfwGetTime();
        m_deltaTime   = nowTime - m_lastTime;
        m_lastTime    = nowTime;
    }

    bool Window::Running() {
        return !glfwWindowShouldClose(m_handle);
    }
    
}