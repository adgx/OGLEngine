#include "TitleScreen.h"
#include "inputManager.h"
#include "windowManager.h"
#include <iostream>
using namespace std;


// Helper to load an image file and create an OpenGL texture. Returns 0 on failure.
static unsigned int loadTextureFromFile(const char* path) {
    int width = 0, height = 0, channels = 4;
    unsigned char* data = stbi_load(path, &width, &height, &channels, 0);
    if (!data) {
        cerr << "Failed to load texture: " << path << endl;
        return 0;
    }        
    GLenum format = GL_RGB;
    unsigned int tex = 0;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

    //glTexParameteri(GL_TEXTURE_2D, ...); // Set texture parameters as needed

    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
    return tex;
}

namespace SpaceEngine{

    TitleScreen::TitleScreen(){
        std::cout << "Title Screen opening..." << std::endl;
    }

    TitleScreen::~TitleScreen(){
        if (texStart)      glDeleteTextures(1, &texStart);
        if (texOptions)    glDeleteTextures(1, &texOptions);
        if (texLeaderboard)glDeleteTextures(1, &texLeaderboard);
        if (texExit)       glDeleteTextures(1, &texExit);
        if (texLogo)       glDeleteTextures(1, &texLogo);
        std::cout << "Going from Title Screen to another scene..." << std::endl;
    }

    void TitleScreen::Init(){
        std::cout << "Loading textures..." << std::endl;

        texStart      = loadTextureFromFile("assets/textures/Startbtn.png");
        texOptions    = loadTextureFromFile("assets/textures/Optionsbtn.png");
        texLeaderboard= loadTextureFromFile("assets/textures/Leaderboardbtn.png");
        texExit       = loadTextureFromFile("assets/textures/Exitbtn.png");
        texLogo       = loadTextureFromFile("assets/textures/Logo.png");

        float btnW = 200.0f; //TODO: cambiare in base alla dimensione dei bottoni
        float btnH = 50.0f;

        float logoW = 400.0f; //TODO: cambiare in base alla dimensione del logo
        float logoH = 100.0f;

        m_buttons.clear();

        float posX = (800.0f - btnW) / 2.0f;
                                            //TODO: cambiare 800 con la larghezza della finestra
        logoX = (800.0f - logoW) / 2.0f;
        logoY = 50.0f;

        m_buttons.push_back({ posX, 200.0f, btnW, btnH, texStart,      TitleResult::PLAY,        false });
        m_buttons.push_back({ posX, 270.0f, btnW, btnH, texOptions,    TitleResult::OPTIONS,     false });
        m_buttons.push_back({ posX, 340.0f, btnW, btnH, texLeaderboard,TitleResult::LEADERBOARD, false });
        m_buttons.push_back({ posX, 410.0f, btnW, btnH, texExit,       TitleResult::EXIT,        false });
        std::cout << "Bottoni caricati: " << m_buttons.size() << std::endl;
    }

    void TitleScreen::Render(){
        cout<<"Rendering elements..."<<endl;
        //TODO
    }

    TitleResult TitleScreen::getInput(){
        cout<<"Managing input from user"<<endl;
        int mx = Mouse::x; 
        int my = Mouse::y;

        for (auto& btn : m_buttons) {
        
        bool isInsideX = (mx >= btn.x && mx <= (btn.x + btn.w));
        bool isInsideY = (my >= btn.y && my <= (btn.y + btn.h));

        if (isInsideX && isInsideY) {
            btn.isHovered = true; //se vogliamo illuminare il bottone al passaggio del mouse
            if(Mouse::buttonDown(0)){
                switch (btn.action) {
                    case TitleResult::PLAY:
                        cout<<"New Game starting..."<<endl;
                        return TitleResult::PLAY;
                        break;

                    case TitleResult::OPTIONS:
                        cout<<"Opening options"<<endl;
                        return TitleResult::OPTIONS;
                        break;

                    case TitleResult::LEADERBOARD:
                        cout<<"Opening Leaderboard"<<endl;
                        return TitleResult::LEADERBOARD;
                        break;

                    case TitleResult::EXIT:
                        cout<<"Quitting Game"<<endl;
                        return TitleResult::EXIT;
                        break;
                        
                    default:
                        break;
                }
            }    
        }else {
            btn.isHovered = false;
        }
        return TitleResult::NONE;
    }  
}
}