#pragma once
#include "GameEnginePrecompiledHeader.h"
#include <GameEngine/core.h>
#include <GameEngine/Events/Event.h>

namespace RendererEngine{
    struct WindowProps{

        WindowProps(const std::string& t="Game Engine", unsigned int w = 1280, unsigned int h = 720) : title(t), width(w), height(h) {}

        std::string  title;
        unsigned int width;
        unsigned int height;
    };

    // Window interface that represents a desktop system based window.
    // Create Window allowing to create windows that are platform specific.
    class RENDER_API Window{
    public:
        using EventCallbackFn = std::function<void(Event& event)>; // Creating a callback function

        virtual ~Window(){}

        virtual void onUpdate() = 0;

        virtual unsigned int getWidth() const = 0;
        virtual unsigned int getHeight() const = 0;

        // Window Attributes
        virtual void setEventCallback(const EventCallbackFn& callback) = 0;
        virtual void setVSync(bool enabled) = 0;
        virtual bool isVSync() const = 0;

        // This will return a GLFW window
        // void* is because this could potentially return an HHWindow, GLFWwindow*, etc.
        virtual void* getNativeWindow() const = 0;

        // Create Window function allowing us to create windows that may be platform specific.
        static Window* create(const WindowProps& props = WindowProps());
    };
};
