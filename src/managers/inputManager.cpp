#include "inputManager.h"
#include "windowManager.h"
#include "log.h"

namespace SpaceEngine
{
    //callbacks
    static void joystick_callback(int jid, int event);
    static void mouse_button_callback(GLFWwindow* window, int buttonID, int action, int mods);
    static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
    static void keyboard_button_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    
    //Mouse
    std::array<bool, SPACE_ENGINE_MOUSE_BUTTON_LAST+1> Mouse::buttons;
    std::array<bool, SPACE_ENGINE_MOUSE_BUTTON_LAST+1> Mouse::buttonsLast;
    int Mouse::x = 0;
    int Mouse::y = 0;
    bool Mouse::cursorHideState = false;

    void Mouse::init()
    {
        showCursor();
        std::fill(buttons.begin(), buttons.end(), false);
        std::fill(buttonsLast.begin(), buttonsLast.end(), false);
    }

    bool Mouse::button(int id)
    {
        if(id >= SPACE_ENGINE_MOUSE_BUTTON_FIRST &&
            id <= SPACE_ENGINE_MOUSE_BUTTON_LAST)
            return buttons[id];
        return false;
    }

    bool Mouse::buttonDown(int id)
    {
        if(id >= SPACE_ENGINE_MOUSE_BUTTON_FIRST &&
            id <= SPACE_ENGINE_MOUSE_BUTTON_LAST)
        {
            if(buttons[id] && !buttonsLast[id])
            {
                buttonsLast[id]=buttons[id];
                return true;
            }
        }
        return false;
    }

    bool Mouse::buttonUp(int id)
    {
        if(id >= SPACE_ENGINE_MOUSE_BUTTON_FIRST &&
            id <= SPACE_ENGINE_MOUSE_BUTTON_LAST)
            return !buttons[id] && buttonsLast[id];
        return false;
    }

    void Mouse::hideCursor()
    {
        glfwSetInputMode(Managers::Window::window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        cursorHideState = true;
    }

    void Mouse::showCursor()
    {
        glfwSetInputMode(Managers::Window::window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        cursorHideState = false;
    }

    bool Mouse::isHidenCursor()
    {
        return cursorHideState;
    }

    //Keyboard
    std::unordered_map<int, bool> Keyboard::keys;
    std::unordered_map<int, bool> Keyboard::keysLast;

    void Keyboard::init()
    {
        //numerical keys
        for(int k = SPACE_ENGINE_KEY_BUTTON_0; k <= SPACE_ENGINE_KEY_BUTTON_9; k++)
        {
            keys[k]=false;
            keysLast[k]=false;
        }

        //alphabetic keys
        for(int k = SPACE_ENGINE_KEY_BUTTON_A; k <= SPACE_ENGINE_KEY_BUTTON_Z; k++)
        {
            keys[k]=false;
            keysLast[k]=false;
        }
        //special keys
        keys[SPACE_ENGINE_KEY_BUTTON_ENTER]=false;
        keys[SPACE_ENGINE_KEY_BUTTON_SPACE]=false;
        keys[SPACE_ENGINE_KEY_BUTTON_BACKSPACE]=false;
        keys[SPACE_ENGINE_KEY_BUTTON_ESCAPE]=false;
        keysLast[SPACE_ENGINE_KEY_BUTTON_ENTER]=false;
        keysLast[SPACE_ENGINE_KEY_BUTTON_SPACE]=false;
        keysLast[SPACE_ENGINE_KEY_BUTTON_BACKSPACE]=false;
        keysLast[SPACE_ENGINE_KEY_BUTTON_ESCAPE]=false;
    }

    bool Keyboard::key(int id)
    {
        if(keys.contains(id))
            return keys[id];
        return false;
    }
    bool Keyboard::keyDown(int id)
    {
        if(keys.contains(id))
            return keys[id] && !keysLast[id];
        return false;
    }
    bool Keyboard::keyUp(int id)
    {
        if(keys.contains(id))
            return keysLast[id] && !keys[id];
        return false;
    }

    //managers::Input
    void Managers::Input::Initialize()
    {
        Mouse::init();
        Keyboard::init();

        //check if the joystick just is present at the startup
        if(glfwJoystickPresent(GLFW_JOYSTICK_1) == GLFW_TRUE)
        {
            SPACE_ENGINE_INFO("Joystick detected: ID={}", GLFW_JOYSTICK_1);
            Joystick::setDeadzone(true);
            Joystick::setClamp(true);
            Joystick::init();
        }
        else SPACE_ENGINE_ERROR("No joystick detected");
        
        //callbacks for handling the input
        glfwSetJoystickCallback(joystick_callback);
        glfwSetMouseButtonCallback(Managers::Window::window, mouse_button_callback);
        glfwSetCursorPosCallback(Managers::Window::window, cursor_position_callback);
        glfwSetKeyCallback(Managers::Window::window, keyboard_button_callback);
    }

    
    void Managers::Input::Update()
    {
        Joystick::update();
    }

    
    void Managers::Input::Shutdown()
    {
        Joystick::destroy();
    }

    //Joystick
    bool Joystick::deadzone = false;
    bool Joystick::clamp = false;
    std::unique_ptr<Joystick::Controller> Joystick::controller = nullptr;

    void Joystick::init()
    {
        SPACE_ENGINE_ASSERT(controller == nullptr, "Controller is not nullptr");
        if(!controller)
        {
            controller = std::make_unique<Controller>();
            controller->cID = 0;
            std::fill(controller->buttons.begin(), controller->buttons.end(), false);
            std::fill(controller->buttonsLast.begin(), controller->buttonsLast.end(), false);
            std::fill(controller->axies.begin(), controller->axies.end(), .0f);
            std::fill(controller->axiesLast.begin(), controller->axiesLast.end(), .0f);
        }
    }

    void Joystick::update()
    {
        if(controller)
        {
            GLFWgamepadstate state;
            if(glfwGetGamepadState(controller->cID, &state))
            {
                for(int i = SPACE_ENGINE_JK_BUTTON_FIRST; i < SPACE_ENGINE_JK_BUTTON_LAST; i++)
                {
                    controller->buttonsLast[i]=controller->buttons[i];
                    controller->buttons[i]=static_cast<bool>(state.buttons[i]);
                }

                for(int i = SPACE_ENGINE_JK_AXIS_FIRST; i < SPACE_ENGINE_JK_AXIS_LAST; i++)
                {
                    controller->axiesLast[i]=controller->axies[i];
                    controller->axies[i]=state.axes[i];
                }
            }
        }
    }

    void Joystick::destroy()
    {
        if(controller)
        {
            controller.reset();
        }
    }  

    bool Joystick::button(int id)
    {
        if(controller)
        {
            if(id>=SPACE_ENGINE_JK_BUTTON_FIRST && id <=SPACE_ENGINE_JK_BUTTON_LAST)
            {
                return controller->buttons[id];
            }
            else SPACE_ENGINE_ERROR("Button is not found");
        }
        return false;
    }

    bool Joystick::buttonDown(int id)
    {
        if(controller)
        {
            if(id>=SPACE_ENGINE_JK_BUTTON_FIRST && id <=SPACE_ENGINE_JK_BUTTON_LAST)
            {
                return controller->buttons[id] && !controller->buttonsLast[id];
            }
            else SPACE_ENGINE_ERROR("Button is not found");
        }
        return false;
    }

    bool Joystick::buttonUp(int id)
    {
        if(controller)
        {
            if(id>=SPACE_ENGINE_JK_BUTTON_FIRST && id <=SPACE_ENGINE_JK_BUTTON_LAST)
            {
                return controller->buttonsLast[id] && !controller->buttons[id];
            }
            else SPACE_ENGINE_ERROR("Button is not found");
        }
        return false;
    }

    float Joystick::axis(int id)
    {
        if(controller)
        {
            if(id>=SPACE_ENGINE_JK_AXIS_FIRST && id <=SPACE_ENGINE_JK_AXIS_LAST)
            {
                float val = controller->axies[id];

                if((deadzone) && std::fabs(val) < deadzoneVal)
                    return 0.f;
                else if(clamp)
                    return (val < 0) ? -1.f : (val > 0) ? 1.f : .0f;
                return val;

            }
            else SPACE_ENGINE_ERROR("Axis is not found");
        }
        return std::numeric_limits<float>::lowest();
    }

    void Joystick::setDeadzone(bool flag)
    {
        deadzone = flag;
    }

    void Joystick::setClamp(bool flag)
    {
        clamp = flag;
    }

    bool Joystick::getDeadzone()
    {
        return deadzone;
    }

    bool Joystick::getClamp()
    {
        return clamp;
    }
    
    //callbacks
    static void joystick_callback(int jid, int event)
    {
        if(jid == 0)
        {
            if(event == GLFW_CONNECTED)
            {
                SPACE_ENGINE_INFO("Joystick connected: ID={}", jid);
                Joystick::init();
            }
            else if(event == GLFW_DISCONNECTED)
            {
                SPACE_ENGINE_ERROR("Joystick disconnected: ID={}", jid);
                Joystick::destroy();
            }
        }
        else if(jid != 0)
        {
            if(event == GLFW_CONNECTED)
            {
                SPACE_ENGINE_ERROR("Joystick connected: ID={} but is not handled", jid);

            }
            else if(event == GLFW_DISCONNECTED)
            {
                SPACE_ENGINE_ERROR("Joystick disconnected: ID={}", jid);
            }
        }
    }

    static void mouse_button_callback(GLFWwindow* window, int buttonID, int action, int mods)
    {
        if(buttonID >= SPACE_ENGINE_MOUSE_BUTTON_FIRST &&
            buttonID <= SPACE_ENGINE_MOUSE_BUTTON_LAST)
        {
            SPACE_ENGINE_DEBUG("buttonsLast: {}, buttons:{}", Mouse::buttonsLast[buttonID], Mouse::buttons[buttonID]);
            Mouse::buttonsLast[buttonID] = Mouse::buttons[buttonID];

            if(action == GLFW_PRESS)
            {
                Mouse::buttons[buttonID] = true;
                SPACE_ENGINE_DEBUG("Mouse button pressed");
            }
            else if(action == GLFW_RELEASE)
            {
                Mouse::buttons[buttonID] = false;
            }
        }
        else SPACE_ENGINE_ERROR("InputManager - Mouse button is not handled");
    }

    static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
    {
        Mouse::x = static_cast<int>(xpos);
        Mouse::y = static_cast<int>(ypos);
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