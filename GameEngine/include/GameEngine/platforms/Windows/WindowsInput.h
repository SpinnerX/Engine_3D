#pragma once
#include <GameEngine/InputPoll.h>

namespace RendererEngine{
    class WindowsInput : public InputPoll{
    protected:
        bool isKeyPressedImpl(int keycode) override;

        bool isMouseButtonPressedImpl(int button) override;

        float getMouseXImpl() override;
        float getMouseYImpl() override;

        MousePosition getMousePositionImpl() override;
    };
};