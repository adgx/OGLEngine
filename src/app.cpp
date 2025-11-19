#include "app.h"
#include "log.h"

namespace SpaceEngine{
    //static functions
    static void joystick_callback(int jid, int event);
    
    App::App()
    {
        //initialize Managers
        logManager.Initialize();
        windowManager.Initialize();
        inputManager.Initialize();
        //Objects
        scene = new Scene();
        renderer = new Renderer();
        SPACE_ENGINE_INFO("Initilization app done");
    }
    
    App::~App()
    {
        //Shutdown Managers
        windowManager.Shutdown();
        inputManager.Shutdown();
        logManager.Shoutdown();
        delete scene;
        delete renderer;
    }

    void App::Run()
    {
        SPACE_ENGINE_DEBUG("App - GameLoop"); 
        while(!windowManager.WindowShouldCLose())
        {
            inputManager.Update();
            if(Mouse::button(SPACE_ENGINE_MOUSE_BUTTON_LEFT))
                SPACE_ENGINE_DEBUG("Left mouse button pressed");
            if(Mouse::button(SPACE_ENGINE_MOUSE_BUTTON_RIGHT))
                SPACE_ENGINE_DEBUG("Right mouse button pressed");
            glClearColor(0.f, 0.f, 0.f, 1.f);
            glClear(GL_COLOR_BUFFER_BIT);

            windowManager.PollEvents();
            windowManager.SwapBuffers();
        }
    }
};
