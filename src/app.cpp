#include "app.h"
#include <iostream>

namespace SpaceEngine{
    App::App()
    {
        setUpGLFW();
        scene = new Scene();
        renderer = new Renderer();
    }
    
    App::~App()
    {
        delete scene;
        delete renderer;
        glfwTerminate();
    }

    void App::Run()
    {
        std::cout << "App - GameLoop" << std::endl; 

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
        std::cout << "App - set up GLFW" << std::endl;
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_PROFILE);

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
