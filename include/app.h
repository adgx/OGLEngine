#pragma once
#include "renderer.h"
#include "scene.h"
#include "managers/logmanager.h"
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
            void handelInput();
            bool setUpGLFW();
        
            //managers
            Managers::LogManager logManager; 
            //Objects
            Scene* scene;
            Renderer* renderer;
            GLFWwindow* window;
    };
};