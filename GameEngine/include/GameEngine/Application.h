#pragma once
#include <memory>
#include <GameEngine/core.h>
#include <GameEngine/Window.h>
#include <GameEngine/LayerStack.h>
#include <GameEngine/Imgui/ImGuiLayer.h>
#include <GameEngine/Renderer/Shader.h>
#include <GameEngine/Renderer/Buffer.h>
#include <GameEngine/Renderer/VertexArray.h>
#include <GameEngine/Renderer/Renderer.h>

namespace RendererEngine{
    // Application
    // - Pure virtual application class
    // - Essentially how users will run their own applications
    class RENDER_API Application{
    public:
        Application();
        ~Application();

        void Run();

        // We can use the EventDispatcher created to dispatch events depending on what kind of
        // events that is needed to dispatched being passed in.
        void onEvent(Event& event);

        void pushLayer(Layer* layer);
        void pushOverlay(Layer* layer);

        // Wherever we are in our codebase, we will want to access the application
        // since this application contain important informaiton
        // Therefore why we only create one instance.
        inline static Application& Get() { return *_instance; }
        inline Window& GetWindow() { return *_window; }


    private:
        bool onWindowClose(WindowCloseEvent& e);

    protected:
        std::unique_ptr<Window> _window; // a unique pointer because this class owns this ptr
        ImGuiLayer* _imguiLayer; // ImGuiLayer class (this class owns this ptr)
        bool isRunning;
        LayerStack _layerStack;

        std::shared_ptr<Shader> _shader;
        std::shared_ptr<VertexArray> _vertexArray;

        std::shared_ptr<Shader> _blueShader;
        std::shared_ptr<VertexArray> _squareVertexArrays;

    private:
        // Since there really is only going to be one application
        static Application* _instance; // Getting our current application instance
    };

    // To be defined in client.
    Application* CreateApplication();
};