#include "app.h"
#include "log.h"
#include <iostream>

namespace SpaceEngine{
    App::App()
    {
        setUpGLFW();
        //initialize Managers
        logManager.Initialize();
        //Objects
        scene = new Scene();
        renderer = new Renderer();
    }
    
    App::~App()
    {
        //Shutdown Managers
        logManager.Shoutdown();
        delete scene;
        delete renderer;
        glfwTerminate();
    }

    void App::Run()
    {
        SPACE_ENGINE_TRACE("App - GameLoop"); 

        while(!glfwWindowShouldClose(window))
        {
            handelInput();
            glClearColor(0.f, 0.f, 0.f, 1.f);
            glClear(GL_COLOR_BUFFER_BIT);

            glfwPollEvents();
            glfwSwapBuffers(window);
        }
    }

    void App::handelInput()
    {
        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
    }

    bool App::setUpGLFW()
    {
        SPACE_ENGINE_TRACE("App - set up GLFW");
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window = glfwCreateWindow(640, 480, "Spaceship", NULL, NULL);

        if(window == nullptr)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            return false;
        }

        glfwMakeContextCurrent(window);

        int version = gladLoadGL(glfwGetProcAddress);

        if(version == 0)
        {
            std::cout << "Failed to initilize OpenGL Context" << std::endl;
            return false;
        }

        glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height){
            glViewport(0, 0, width, height);
        });

        std::cout<< "App - GLFW setup done" << std::endl;
        return true;
    }

};
