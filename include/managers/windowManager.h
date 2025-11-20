#pragma once
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#define SPACE_ENGINE_MIN_RES_W 720
#define SPACE_ENGINE_MIN_RES_H 480
#define SPACE_ENGINE_MAX_RES_W 1920
#define SPACE_ENGINE_MAX_RES_H 1080


namespace SpaceEngine::Managers
{
    class Window
    {
        public:
            Window() = default;
            ~Window() = default;

            void Initialize();
            int WindowShouldClose();
            void SetWindowShouldClose();
            void Windowed();
            void PollEvents();
            void SwapBuffers();
            void Shutdown();

            static int height, width;
            static int xpos, ypos;
            static float aspectRatio;
            static GLFWwindow* window;
            static GLFWmonitor* monitor;
            static bool fullScreenState;
            constexpr static const float aspectRatio = 16/9;  
        private:
            bool setUpGLFW();
    };
}

