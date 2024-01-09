#include <GameEnginePrecompiledHeader.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <GameEngine/Imgui/ImGuiLayer.h>
#include <GameEngine/Core/Application.h>
#include <GameEngine/Core/core.h>
#include <GameEngine/Core/KeyCodes.h>

namespace RendererEngine{
    ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer") {
    }

    ImGuiLayer::~ImGuiLayer() {}

    void ImGuiLayer::onAttach(){
		RENDER_PROFILE_FUNCTION();

        // IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark(); // using dark colors

        ImGuiIO& io = ImGui::GetIO(); (void)io;
        
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
        //io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable){
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

        // Initializing the OpenGL3 ()
        GLFWwindow* window = static_cast<GLFWwindow *>(Application::Get().GetWindow().getNativeWindow());

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 410"); // We should check the version of GLSL (if there is an error with this version)
    }
    
    void ImGuiLayer::onDetach(){
		RENDER_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
    }

    void ImGuiLayer::begin(){
		RENDER_PROFILE_FUNCTION();

        ImGui_ImplGlfw_NewFrame();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();
    }

    void ImGuiLayer::end(){
		RENDER_PROFILE_FUNCTION();

        ImGuiIO& io = ImGui::GetIO();

        Application& app = Application::Get();
        io.DisplaySize = ImVec2(app.GetWindow().getWidth(), app.GetWindow().getHeight());

        // We want this part to be controlled by the layers themselves.
        // (or whatever layer we have inside Sandbox)
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable){
            GLFWwindow* backup_current_ctx = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            
            glfwMakeContextCurrent(backup_current_ctx);
        }
    }
};
