#include "inputManager.h"
#include "windowManager.h"
#include "log.h"

namespace SpaceEngine
{
    //callbacks
    static void joystick_callback(int jid, int event);
    static void mouse_button_callback(GLFWwindow* window, int buttonID, int action, int mods);
    static void keyboard_button_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    
    //Mouse
    int Mouse::x = 0;
    int Mouse::y = 0;
    std::array<bool, SPACE_ENGINE_MOUSE_BUTTON_LAST+1> Mouse::buttons;
    std::array<bool, SPACE_ENGINE_MOUSE_BUTTON_LAST+1> Mouse::buttonsLast;
    
    void Mouse::init()
    {
        std::fill(Mouse::buttons.begin(), Mouse::buttons.end(), false);
        std::fill(Mouse::buttonsLast.begin(), Mouse::buttonsLast.end(), false);
    }

    bool Mouse::button(int id)
    {
        if(id >= SPACE_ENGINE_MOUSE_BUTTON_FIRST &&
            id <= SPACE_ENGINE_MOUSE_BUTTON_LAST)
            return Mouse::buttons[id];
        return false;
    }

    bool Mouse::buttonDown(int id)
    {
        if(id >= SPACE_ENGINE_MOUSE_BUTTON_FIRST &&
            id <= SPACE_ENGINE_MOUSE_BUTTON_LAST)
            return Mouse::buttons[id] && !Mouse::buttonsLast[id];
        return false;
    }

    bool Mouse::buttonUp(int id)
    {
        if(id >= SPACE_ENGINE_MOUSE_BUTTON_FIRST &&
            id <= SPACE_ENGINE_MOUSE_BUTTON_LAST)
            return !Mouse::buttons[id] && Mouse::buttonsLast[id];
        return false;
    }
    //Keyboard
    std::unordered_map<int, bool> Keyboard::keys;
    std::unordered_map<int, bool> Keyboard::keysLast;

    void Keyboard::init()
    {
        //numerical keys
        for(int k = SPACE_ENGINE_KEY_BUTTON_0; k <= SPACE_ENGINE_KEY_BUTTON_9; k++)
        {
            Keyboard::keys[k]=false;
            Keyboard::keysLast[k]=false;
        }

        //alphabetic keys
        for(int k = SPACE_ENGINE_KEY_BUTTON_A; k <= SPACE_ENGINE_KEY_BUTTON_Z; k++)
        {
            Keyboard::keys[k]=false;
            Keyboard::keysLast[k]=false;
        }
        //special keys
        Keyboard::keys[SPACE_ENGINE_KEY_BUTTON_ENTER]=false;
        Keyboard::keys[SPACE_ENGINE_KEY_BUTTON_SPACE]=false;
        Keyboard::keys[SPACE_ENGINE_KEY_BUTTON_BACKSPACE]=false;
        Keyboard::keysLast[SPACE_ENGINE_KEY_BUTTON_ENTER]=false;
        Keyboard::keysLast[SPACE_ENGINE_KEY_BUTTON_SPACE]=false;
        Keyboard::keysLast[SPACE_ENGINE_KEY_BUTTON_BACKSPACE]=false;
    }

    bool Keyboard::key(int id)
    {
        if(keys.contains(id))
            return Keyboard::keys[id];
        return false;
    }
    bool Keyboard::keyDown(int id)
    {
        if(keys.contains(id))
            return Keyboard::keys[id] && !Keyboard::keysLast[id];
        return false;
    }
    bool Keyboard::keyUp(int id)
    {
        if(keys.contains(id))
            return Keyboard::keysLast[id] && !Keyboard::keys[id];
        return false;
    }
    //managers::Input
    void Managers::Input::Initialize()
    {
        Mouse::init();
        Keyboard::init();

        if(glfwJoystickPresent(GLFW_JOYSTICK_1) == GLFW_TRUE)
        {
            SPACE_ENGINE_INFO("Joystick detected: ID={}", GLFW_JOYSTICK_1);
        }
        else SPACE_ENGINE_ERROR("No joystick detected");
        
        //callbacks for handling the input
        glfwSetJoystickCallback(joystick_callback);
        glfwSetMouseButtonCallback(Managers::Window::window, mouse_button_callback);
    }

    
    void Managers::Input::Update()
    {
        
    }

    
    void Managers::Input::Shutdown()
    {

    }

    //callbacks
    static void joystick_callback(int jid, int event)
    {
        if(event == GLFW_CONNECTED)
        {
            SPACE_ENGINE_INFO("Joystick connected: ID={}", jid);
        }
        else if(event == GLFW_DISCONNECTED)
        {
            SPACE_ENGINE_ERROR("Joystick disconnected: ID={}", jid);
        }
    }

    static void mouse_button_callback(GLFWwindow* window, int buttonID, int action, int mods)
    {
        if(buttonID >= SPACE_ENGINE_MOUSE_BUTTON_FIRST &&
            buttonID <= SPACE_ENGINE_MOUSE_BUTTON_LAST)
        {
            Mouse::buttonsLast[buttonID] = Mouse::buttons[buttonID];

            if(action == GLFW_PRESS)
            {
                Mouse::buttons[buttonID] = true;
            }
            else if(action == GLFW_RELEASE)
            {
                Mouse::buttons[buttonID] = false;
            }
        }
        else SPACE_ENGINE_ERROR("InputManager - Mouse button is not handled");
    }

    static void keyboard_button_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if(Keyboard::keys.contains(key))
        {
            Keyboard::keysLast[key] = Keyboard::keys[key];

            if(action == GLFW_PRESS)
            {
                Keyboard::keys[key] = true;
            }
            else if(action == GLFW_RELEASE)
            {
                Keyboard::keys[key] = false;
            }
        }
        else SPACE_ENGINE_ERROR("InputManager - Key is not handled");
    }
}