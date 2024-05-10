#pragma once
#include <Engine3D/Core/core.h>
#include <Engine3D/Event/Event.h>

namespace Engine3D{
    struct WindowProps{

        WindowProps(const std::string& t="Game Engine", uint32_t w = 1600, uint32_t h = 900) : title(t), width(w), height(h) {}

        std::string  title;
        uint32_t width;
        uint32_t height;
    };

    // Window interface that represents a desktop system based window.
    // Create Window allowing to create windows that are platform specific.
    class ENGINE_API Window{
    public:
        using EventCallbackFn = std::function<void(Event& event)>; // Creating a callback function

        virtual ~Window(){}

        virtual void OnUpdate() = 0;

        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const = 0;

        // Window Attributes
        virtual void setEventCallback(const EventCallbackFn& callback) = 0;
        virtual void setVSync(bool enabled) = 0;
        virtual bool isVSync() const = 0;

        // This will return a GLFW window
        // void* is because this could potentially return an HHWindow, GLFWwindow*, etc.
        virtual void* GetNativeWindow() const = 0;

        // Create Window function allowing us to create windows that may be platform specific.
        static Window* create(const WindowProps& props = WindowProps());
    };
};