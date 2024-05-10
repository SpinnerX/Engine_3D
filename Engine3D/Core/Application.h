#pragma once
#include <Engine3D/Core/core.h>
#include <Engine3D/Core/Window.h>
#include <Engine3D/Core/LayerStack.h>
#include <Engine3D/Imgui/ImGuiLayer.h>
#include <Engine3D/Event/ApplicationEvent.h>

namespace Engine3D{
	struct ApplicationCommandLineArgs{
		int count = 0;
		char** args = nullptr;

		const char* operator[](int index) const {
			assert(index < count);
			return args[index];
		}
	};

    // Application
    // - Essentially how users will run their own applications
    class Application{
    public:
        /* Application(const std::string& name="Game Engine"); */
		Application(const std::string& name="Game Engine", ApplicationCommandLineArgs args = ApplicationCommandLineArgs{});
        ~Application();

        void Run();

        // We can use the EventDispatcher created to dispatch events depending on what kind of
        // events that is needed to dispatched being passed in.
        void OnEvent(Event& event);

        void pushLayer(Layer* layer);
        void pushOverlay(Layer* layer);
		
		void close();
		
		static ImGuiLayer* GetImGuiLayer() { return _instance->_imguiLayer; }

        // Wherever we are in our codebase, we will want to access the application
        // since this application contain important informaiton
        // Therefore why we only create one instance.
        // inline static Application& Get() { return *_instance; }

        inline static ApplicationCommandLineArgs GetCmdLineArg() { return _instance->GetCommandLineArgs(); }

        inline static void Close() { _instance->close(); }

        inline static Window& Get() { return *_instance->_window; }

        // inline Window& GetWindow() { return *_window; }

		ApplicationCommandLineArgs GetCommandLineArgs() const { return _commandLineArgs; }

    private:
        bool onWindowClose(WindowCloseEvent& e);
        bool onWindowResize(WindowResizeEvent& e);

    private:
        std::unique_ptr<Window> _window; // a unique pointer because this class owns this ptr
        ImGuiLayer* _imguiLayer; // ImGuiLayer class (this class owns this ptr)
        bool isRunning;
		bool isMinimized;
        LayerStack _layerStack;

        float _lastFrameTime = 0.0f; // Time it took to render the last frame
		ApplicationCommandLineArgs _commandLineArgs;
    private:
        // Since there really is only going to be one application
        static Application* _instance; // Getting our current application instance
    };

    // To be defined in client.
    Application* CreateApplication(ApplicationCommandLineArgs args);
};
