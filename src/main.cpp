#include <iostream>
#include "app.h"

int main()
{
    std::cout << "Launcher" << std::endl;
    
    SpaceEngine::App* app = new SpaceEngine::App();
    app->Run();
    
    delete app;

    return 0;
}