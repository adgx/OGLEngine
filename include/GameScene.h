#pragma once
#include "utils/stb_image.h"
#include <vector>
#include <string>

namespace SpaceEngine{
    class GameScene{
    public:
        GameScene();
        ~GameScene();

        unsigned int LoadCubemap(std::vector<std::string> faces);
        void Init();
        void Render();
        void Update();
    private:
        unsigned int cubemapTexture;
        std::vector<std::string> faces = {            //TODO: sistemare i path con i file giusti
            "assets/textures/skybox/right.jpg",
            "assets/textures/skybox/left.jpg",
            "assets/textures/skybox/top.jpg",
            "assets/textures/skybox/bottom.jpg",
            "assets/texture/skybox/front.jpg",
            "assets/textures/skybox/back.jpg"
        };
    };
}