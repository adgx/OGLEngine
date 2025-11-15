#pragma once
#include "renderer.h"
#include "scene.h"
#include <glad/gl.h>
#include <GLFW/glfw3.h>

namespace SpaceEngine
{
    class App
    {
        public:
            App();
            ~App();
            void Run();
        private:
            bool handelInput();
            bool setupGLFW();
        
        Scene* scene;
        Renderer* renderer;
        GLFWwindow* window;
    };
};