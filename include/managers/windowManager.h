#pragma once
#include <glad/gl.h>
#include <GLFW/glfw3.h>
namespace SpaceEngine::Managers
{
    class Window
    {
        public:
            Window() = default;
            ~Window() = default;

            void Initialize();
            int WindowShouldCLose();
            void PollEvents();
            void SwapBuffers();
            void Shutdown();

            static GLFWwindow* window;
        private:
            bool setUpGLFW();
    };
}

