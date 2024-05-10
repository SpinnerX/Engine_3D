#pragma once
#include <Engine3D/Core/Window.h>
#include <Engine3D/Graphics/GraphicsContext.h>

struct GLFWwindow;

namespace Engine3D{
    // Windows OS specific Window
    class WindowsWindow : public Window{
    public:
        WindowsWindow(const WindowProps& props);
        virtual ~WindowsWindow();

        // Should just update GLFW
        // including swapping buffers, input events, and things like that
        // Should be running once-per frame
        void OnUpdate() override;

        inline uint32_t GetWidth() const override { return _data.width; }
        inline uint32_t GetHeight() const override { return _data.height; }

        // Window attributes
        void setEventCallback(const EventCallbackFn& callback) override {
            _data.callback = callback;
        }

        void setVSync(bool enabled) override;
        bool isVSync() const override { return _data.vSync; }

        inline void* GetNativeWindow() const override {
            return _window;
        }

    private:
        virtual void init(const WindowProps& props);
        virtual void shutdown();

    private:
        GLFWwindow* _window;
        Scope<GraphicsContext> _context; // Essentially our GraphicsContext

        // Window data
        // For containing all relative information to thhe window
        // This is so we can pass custom user WindowData to GLFW instead of passing in the entire class
        struct WindowData{
            std::string title;
            unsigned width, height;
            bool vSync;
            EventCallbackFn callback;
        };

        WindowData _data;

    };
};
