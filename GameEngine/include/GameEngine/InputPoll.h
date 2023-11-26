#pragma once
#include <GameEngine/core.h>

// InputPoll is going to be static
// - Where in a sense that there is going to be only one global input kind of manager
// - InputPoll is an interface allowing users to their own input poll systems
// - That can be used to check if a key or multiple keys has been pressed.

namespace RendererEngine{
    struct MousePosition{
        float xpos = 0.f;
        float ypos = 0.f;
    };
    class RENDER_API InputPoll{
    public:
        // Taking in a key and checking if it has been pressed
        // creating a singletone called _instance
        // This calls the implementation for Key Pressed that is implemented in WindowsInput
        inline static bool isKeyPressed(int keycode){
            return _instance->isKeyPressedImpl(keycode);
        }

        inline static MousePosition getMousePosition() {
            return _instance->getMousePositionImpl();
        }

        inline static bool isMouseButtonPressed(int button){
            return _instance->isMouseButtonPressedImpl(button);
        }

        inline static float getMouseX(){
            return _instance->getMouseXImpl();
        }

        inline static float getMouseY(){
            return _instance->getMouseYImpl();
        }

    protected:
        // This will be implemented by your interface
        virtual bool isKeyPressedImpl(int keycode) = 0;
        virtual bool isMouseButtonPressedImpl(int button) = 0;

        virtual MousePosition getMousePositionImpl() = 0;
        virtual float getMouseXImpl() = 0;
        virtual float getMouseYImpl() = 0;

    private:
        // Essentially this is just to pass in data to these functions
        static InputPoll* _instance;
    };
};