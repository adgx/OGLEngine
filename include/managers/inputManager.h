#pragma once
#include <array>
#include <unordered_map>

namespace SpaceEngine::Managers
{

    class GameObject;
    class Input
    {
        public:
            void Initialize();
            void Update();
            void Shutdown();
    };
    class Command
    {
        public:
        virtual ~Command(){}
        virtual void Execute(GameObject gameobj) = 0;
    };

    class InputHandler
    {

    };



};

enum
{
    SPACE_ENGINE_MOUSE_BUTTON_FIRST,
    SPACE_ENGINE_MOUSE_BUTTON_LEFT = SPACE_ENGINE_MOUSE_BUTTON_FIRST,
    SPACE_ENGINE_MOUSE_BUTTON_RIGHT,
    SPACE_ENGINE_MOUSE_BUTTON_MIDDLE,
    SPACE_ENGINE_MOUSE_BUTTON_LAST = SPACE_ENGINE_MOUSE_BUTTON_MIDDLE,
    SPACE_ENGINE_KEY_BUTTON_0=48,
    SPACE_ENGINE_KEY_BUTTON_1=49,
    SPACE_ENGINE_KEY_BUTTON_2=50,
    SPACE_ENGINE_KEY_BUTTON_3=51,
    SPACE_ENGINE_KEY_BUTTON_4=52,
    SPACE_ENGINE_KEY_BUTTON_5=53,
    SPACE_ENGINE_KEY_BUTTON_6=54,
    SPACE_ENGINE_KEY_BUTTON_7=55,
    SPACE_ENGINE_KEY_BUTTON_8=56,
    SPACE_ENGINE_KEY_BUTTON_9=57,
    SPACE_ENGINE_KEY_BUTTON_A=65,
  	SPACE_ENGINE_KEY_BUTTON_B=66,
  	SPACE_ENGINE_KEY_BUTTON_C=67,
  	SPACE_ENGINE_KEY_BUTTON_D=68,
  	SPACE_ENGINE_KEY_BUTTON_E=69,
  	SPACE_ENGINE_KEY_BUTTON_F=70,
  	SPACE_ENGINE_KEY_BUTTON_G=71,
  	SPACE_ENGINE_KEY_BUTTON_H=72,
  	SPACE_ENGINE_KEY_BUTTON_I=73,
  	SPACE_ENGINE_KEY_BUTTON_J=74,
  	SPACE_ENGINE_KEY_BUTTON_K=75,
  	SPACE_ENGINE_KEY_BUTTON_L=76,
  	SPACE_ENGINE_KEY_BUTTON_M=77,
  	SPACE_ENGINE_KEY_BUTTON_N=78,
  	SPACE_ENGINE_KEY_BUTTON_O=79,
  	SPACE_ENGINE_KEY_BUTTON_P=80,
  	SPACE_ENGINE_KEY_BUTTON_Q=81,
  	SPACE_ENGINE_KEY_BUTTON_R=82,
  	SPACE_ENGINE_KEY_BUTTON_S=83,
  	SPACE_ENGINE_KEY_BUTTON_T=84,
  	SPACE_ENGINE_KEY_BUTTON_U=85,
  	SPACE_ENGINE_KEY_BUTTON_V=86,
  	SPACE_ENGINE_KEY_BUTTON_W=87,
  	SPACE_ENGINE_KEY_BUTTON_X=88,
  	SPACE_ENGINE_KEY_BUTTON_Y=89,
    SPACE_ENGINE_KEY_BUTTON_Z=90,
    SPACE_ENGINE_KEY_BUTTON_ENTER=257,
    SPACE_ENGINE_KEY_BUTTON_SPACE=32,
    SPACE_ENGINE_KEY_BUTTON_BACKSPACE=259
};

namespace SpaceEngine
{
    class Mouse
    {
        public:
        static void init();
        //static void update();
        inline static int getPosX() { return x;}
        inline static int getPosY() {return y;}
        static std::array<bool, SPACE_ENGINE_MOUSE_BUTTON_LAST+1> buttons;
        static std::array<bool, SPACE_ENGINE_MOUSE_BUTTON_LAST+1> buttonsLast;
        static bool button(int id);
        static bool buttonDown(int id);
        static bool buttonUp(int id);
        private:
        constexpr static const int ButtonCount = 3;

        static int x;
        static int y;

        
    };

    class Keyboard
    {
        public:
        static void init();
        //static void update();
        static std::array<bool, SPACE_ENGINE_MOUSE_BUTTON_LAST+1> keys;
        static std::array<bool, SPACE_ENGINE_MOUSE_BUTTON_LAST+1> keysLast;
        static bool keys(int id);
        static bool keysDown(int id);
        static bool keysUp(int id);
        private:
        constexpr static const int ButtonCount = 3;       
    };
}