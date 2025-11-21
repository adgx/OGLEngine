#include "windowManager.h"
#include "log.h"

namespace SpaceEngine::Managers
{
    void window_maximize_callback(GLFWwindow* window, int maximized);

    int Window::height = SPACE_ENGINE_MIN_RES_H, Window::width = SPACE_ENGINE_MIN_RES_W;
    int Window::xpos = 0, Window::ypos = 0;
    GLFWwindow* Window::window = nullptr;
    bool Window::fullScreenState = false;
    GLFWmonitor* Window::monitor = nullptr;

    void Window::Initialize()
    {
        setUpGLFW();
    }

    int Window::WindowShouldClose()
    {
        return glfwWindowShouldClose(window);
    }

    void Window::SetWindowShouldClose()
    {
        glfwSetWindowShouldClose(window, true);
    }
    
    void Window::PollEvents()
    {
        glfwPollEvents();
    }
    void Window::Windowed()
    {
        if(fullScreenState)
        {
            fullScreenState = false;
            glfwSetWindowMonitor(Window::window,
		                        nullptr,
		                        Window::xpos,
		                        Window::ypos,
		                        SPACE_ENGINE_MIN_RES_W,
                                SPACE_ENGINE_MIN_RES_H,
		                        0);
            glViewport(0, 0, SPACE_ENGINE_MIN_RES_W, SPACE_ENGINE_MIN_RES_H);
        }
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

        window = glfwCreateWindow(width, height, "Spaceship", NULL, NULL);
        
        //setup window
        glfwSetWindowSizeLimits(window, SPACE_ENGINE_MIN_RES_W, SPACE_ENGINE_MIN_RES_H, GLFW_DONT_CARE, GLFW_DONT_CARE);
        glfwSetWindowAspectRatio(window, 16, 9);
        glfwSetWindowMaximizeCallback(window, window_maximize_callback);

        //saving the position where is created
        glfwGetWindowPos(window, &xpos, &ypos);

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
        //set the OpenGL depth buffer
        glEnable(GL_DEPTH_TEST);
        //set the OpenGL framebuffer
        glViewport(0, 0, SPACE_ENGINE_MIN_RES_W, SPACE_ENGINE_MIN_RES_H); 
        //callbacks
        glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height){
            glViewport(0, 0, width, height);
        });

        SPACE_ENGINE_INFO("WindowManager - GLFW setup done");
        return true;
    }

    void Window::Shutdown()
    {
        glfwTerminate();
    }

    GLFWmonitor* get_window_monitor(GLFWwindow* window)
    {
        int wx, wy, ww, wh;
        glfwGetWindowPos(window, &wx, &wy);
        glfwGetWindowSize(window, &ww, &wh);

        int monitorCount;
        GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);
        GLFWmonitor* bestMonitor = glfwGetPrimaryMonitor();
        
        if(monitorCount == 1)
            return bestMonitor;

        int bestOverlap = 0;

        for (int i = 0; i < monitorCount; i++) {
            int mx, my, mw, mh;
            glfwGetMonitorWorkarea(monitors[i], &mx, &my, &mw, &mh);

            // Compute overlap area
            int overlapW = std::max(0, std::min(wx + ww, mx + mw) - std::max(wx, mx));
            int overlapH = std::max(0, std::min(wy + wh, my + mh) - std::max(wy, my));
            int overlap = overlapW * overlapH;

            if (overlap > bestOverlap) {
                bestOverlap = overlap;
                bestMonitor = monitors[i];
            }
        }

        return bestMonitor;
    }

    //callbacks
    void window_maximize_callback(GLFWwindow* window, int maximized)
    {
        if (maximized)
        {
            // The window was maximized
            Window::fullScreenState = true;
            Window::monitor = get_window_monitor(window);
            const GLFWvidmode* mode = glfwGetVideoMode(Window::monitor);
            glfwSetWindowMonitor(window, Window::monitor, 0, 0, SPACE_ENGINE_MAX_RES_W, SPACE_ENGINE_MAX_RES_H, mode->refreshRate);
            glViewport(0, 0, SPACE_ENGINE_MAX_RES_W, SPACE_ENGINE_MAX_RES_H);
        }
        else
        {
            // The window was restored
        }


    }

}
