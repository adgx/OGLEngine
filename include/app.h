#pragma once
#include "renderer.h"
#include "scene.h"
#include "managers/logManager.h"
#include "managers/inputManager.h"
#include "managers/windowManager.h"
#include <glad/gl.h>

namespace SpaceEngine
{
    class App
    {
        public:
            App();
            ~App();
            void Run();
        private:
            //managers
            Managers::LogManager logManager;
            Managers::Input inputManager;
            //Objects
            Scene* scene;
            Renderer* renderer;
            Managers::Window windowManager;
    };
};