#pragma once

namespace SpaceEngine
{
    class GameObject;
    
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

