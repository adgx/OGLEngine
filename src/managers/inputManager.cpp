#include "inputManager.h"
#include "windowManager.h"
#include "log.h"

namespace SpaceEngine
{
    //callbacks
    static void joystick_callback(int jid, int event);
    static void mouse_button_callback(GLFWwindow* window, int buttonID, int action, int mods);

    int Mouse::x = 0;
    int Mouse::y = 0;

    std::array<bool, SPACE_ENGINE_MOUSE_BUTTON_LAST+1> Mouse::buttons;
    std::array<bool, SPACE_ENGINE_MOUSE_BUTTON_LAST+1> Mouse::buttonsLast;
    
    void Mouse::init()
    {
        std::fill(Mouse::buttons.begin(), Mouse::buttons.end(), false);
        std::fill(Mouse::buttonsLast.begin(), Mouse::buttonsLast.end(), false);
    }
    
    /*
    void Mouse::update()
    {
        double x, y;
        glfwGetCursorPos(Managers::Window::window, &x, &y);

        for(int i =0; i < SPACE_ENGINE_MOUSE_BUTTON_LAST+1; i++)
        {
            Mouse::buttons[i] = static_cast<bool>(glfwGetMouseButton(Managers::Window::window, i));
        }
    }*/

    void Managers::Input::Initialize()
    {
        Mouse::init();

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
        //Mouse::update();
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

    void Managers::Input::Shutdown()
    {

    }

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
    }
}