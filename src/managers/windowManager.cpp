#include "windowManager.h"
#include "log.h"

namespace SpaceEngine::Managers
{
    GLFWwindow* Window::window = nullptr;
    void Window::Initialize()
    {
        setUpGLFW();
    }

    int Window::WindowShouldCLose()
    {
        return glfwWindowShouldClose(window);
    }

    void Window::PollEvents()
    {
        glfwPollEvents();
    }

    void Window::SwapBuffers()
    {
        glfwSwapBuffers(window);
    }
    bool Window::setUpGLFW()
    {
        SPACE_ENGINE_TRACE("App - set up GLFW");
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window = glfwCreateWindow(640, 480, "Spaceship", NULL, NULL);

        if(window == nullptr)
        {
            SPACE_ENGINE_ERROR("Failed to create GLFW window");
            return false;
        }

        glfwMakeContextCurrent(window);

        int version = gladLoadGL(glfwGetProcAddress);

        if(version == 0)
        {
            SPACE_ENGINE_ERROR("Failed to initilize OpenGL Context");
            return false;
        }

        //callbacks
        glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height){
            glViewport(0, 0, width, height);
        });
        //glfwSetJoystickCallback(joystick_callback);

        SPACE_ENGINE_INFO("WindowManager - GLFW setup done");
        return true;
    }

    void Window::Shutdown()
    {
        glfwTerminate();
    }
}
