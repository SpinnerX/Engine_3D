#include <Engine3D/Engine3DPrecompiledHeader.h>
#include <Engine3D/Core/Application.h>
#include <Engine3D/Event/KeyCodes.h>
#include <Engine3D/Event/MouseCodes.h>
#include <Engine3D/Renderer2D/EditorCamera.h>
#include <Engine3D/Renderer2D/Renderer2D.h>
#include <Engine3D/Scene2D/Components.h>
#include <imgui/imgui.h>
#include "EditorLayer.h"

//! @note UI-related toolings
//! @note TODO --- Internal UI Toolkit wrappers around ImGui
//! @note Specific for Engine3D's API
#include "UI/UI.h"


namespace Engine3D{
    EditorLayer::EditorLayer() : Layer("UI Editor"){
    }

    void EditorLayer::OnAttach() {
        ENGINE_PROFILE_FUNCTION();
        currentScene = CreateRef<Scene>();
        playIcon = Texture2D::Create("Resources/icons/PlayButton.png");
        stopIcon = Texture2D::Create("Resources/icons/StopButton.png");

        if(!playIcon->IsLoaded()){
            coreLogWarn("Could not load play icon!");
        }

        if(!stopIcon->IsLoaded()){
            coreLogWarn("Could not load stop icon!");
        }

        FrameBufferSpecifications frameSpecifications;
        // @note In graphics, there are different formats OpenGL handles RGB, such as RGBA8, RED (same as RGBA8 but one channel that is an int.)
		frameSpecifications.attachments = { FrameBufferTextureFormat::RGBA8, FrameBufferTextureFormat::RED_INTEGER, FrameBufferTextureFormat::DEPTH24STENCIL8 };
        frameSpecifications.width = 1280;
        frameSpecifications.height = 720;

        frameBuffer = FrameBuffer::Create(frameSpecifications);
        currentScene = CreateRef<Scene>();

        editorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);

        auto cmdLineArgs = Application::GetCmdLineArg();

        if(cmdLineArgs.count != 1){
            auto scenePath = cmdLineArgs[1];
            SceneSerializer serializer(currentScene);
            serializer.deserializeRuntime(scenePath);
        }

        sceneHeirarchy.SetNewScene(currentScene);


    }

    void EditorLayer::OnDetach(){
        ENGINE_PROFILE_FUNCTION();
    }

    void EditorLayer::OnUpdate(Timestep ts){
        ENGINE_PROFILE_FUNCTION();

        if(FrameBufferSpecifications spec = frameBuffer->getSpecifications();
            viewportSize.x > 0.0f and viewportSize.y > 0.0f and (spec.width != viewportSize.x || spec.height != viewportSize.y)){
            frameBuffer->resize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
            // viewportSize = {viewport}
        }

    }

    void EditorLayer::OnEvent(Event& event){
        ENGINE_PROFILE_FUNCTION();
        EventDispatcher dispatcher(event);

        dispatcher.Dispatch<KeyPressedEvent>([this](KeyPressedEvent& e){
            if(e.GetRepeatedCount() > 0) return false;
            if(InputPoll::IsKeyPressed(ENGINE_KEY_ESCAPE)) Application::Close();

            //! @note Our super key on mac is command.
            bool ctrl = InputPoll::IsKeyPressed(ENGINE_KEY_LEFT_SUPER);

            switch (e.GetKeyCode()) {
            default: break;
            }

            return false;
        });

        dispatcher.Dispatch<MouseButtonPressedEvent>([](MouseButtonPressedEvent& e){
            
            if(InputPoll::IsMousePressed(ENGINE_MOUSE_BUTTON_LEFT)){
                if(UI::IsViewortHovered() && !ImGuizmo::IsOver() && !InputPoll::IsKeyPressed(ENGINE_KEY_LEFT_ALT)){
                    // ... Handling Scenes/Scene-Graph-Related Logic
                }
            }
            return false;
        });
    }

    void EditorLayer::OnUIRender(){
        ENGINE_PROFILE_FUNCTION();

        bool dockSpaceOpened = true;
        bool opt_fullscreen = true;
        bool opt_padding = false;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

        if(opt_fullscreen){
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }
        else{
            dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
        }

        if(dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;
            
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", &dockSpaceOpened, window_flags);
		ImGui::PopStyleVar();

        if(opt_fullscreen)
			ImGui::PopStyleVar(2);

        // Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		float minWinSizeX = 370.0f;
		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowMinSize.x = 370.0f;
		

		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable){
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
		style.WindowMinSize.x = minWinSizeX;

        //! @note Setting up our gui for loading,creating, and saving scenes for IO
        UI::UI_Menubar([this](){
            if (ImGui::BeginMenu("File")){
				
				if(ImGui::MenuItem("New", "Ctrl+N")){
					CreateScene();
				}

				ImGui::Separator();

				if(ImGui::MenuItem("Open", "Ctrl+O")){
					// LoadScene();
				}
				
				ImGui::Separator();

				if(ImGui::MenuItem("Save as", "Ctrl+Shift+s")){
					// SaveScene();
				}
				
				ImGui::Separator();


				if(ImGui::MenuItem("Exit", "Ctrl+X")) Application::Close();

				ImGui::EndMenu();
			}
        });

        sceneHeirarchy.OnUIRender();


        //! @note Currently we create our widgets and we just submit the logic as a lambda
        //! @note This way we have full control of our logic, and not to worry about the boilerplate that comes with ImGui
        UI::UI_CreatePanel("Debug Renderer", [this](){
            std::string name = "No Entity Selected";
            if(hoveredEdity){
                name = hoveredEdity.GetComponent<TagComponent>().tag;
            }

            ImGui::Text("Hovered Entity: %s", name.c_str());

            auto debugRendererSpec = Renderer2D::getStats();

            ImGui::Text("Renderer2D Stats");
            ImGui::Text("Draw Calls %d", debugRendererSpec.drawCalls);
            ImGui::Text("Quads: %d", debugRendererSpec.quadCount);
            ImGui::Text("Vertices: %d", debugRendererSpec.getTotalVertexCount());
            ImGui::Text("Indices: %d", debugRendererSpec.getTotalIndexCount());
        });

        isViewportFocused = UI::IsWindowFocused();
        isViewportHovered = UI::IsWindowHovered();

        UI::UI_CreatePanel("Viewport", [this](){
            //! @note Used for testing our vec3 controller.
            // glm::vec3 values(0.0f, 0.0f, 0.0f);
            // UI::UI_DrawVec3Controller("Transform", values);

            //! @note Used for testing our input text.
            //! @note We assign a default name to our entities
            //! @note This will be used for 
            // Entity entity;
            // UI::UI_InputText("##Tag", entity);

            auto viewportMinReg = ImGui::GetWindowContentRegionMin();
            auto viewportMaxReg = ImGui::GetWindowContentRegionMax();

            auto viewportOffset = ImGui::GetWindowPos();

            viewportBound[0] = {viewportMinReg.x + viewportOffset.x, viewportMinReg.y + viewportOffset.y};
            viewportBound[1] = {viewportMaxReg.x + viewportOffset.x, viewportMaxReg.y + viewportOffset.y};

            //! @note Making sure that our events do not get blocked in any sort of way
            Application::GetImGuiLayer()->SetBlockEvents(!isViewportFocused && !isViewportHovered);

            ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail(); // Should just give us the size of available panel size
            viewportSize = {viewportSize.x, viewportSize.y};
            // Assuming the viewPortPanelSize is this type.
            if(viewportSize != *((glm::vec2 *)&viewportPanelSize) && viewportPanelSize.x > 0 && viewportPanelSize.y > 0){
                // Recreating the frame buffer.
                frameBuffer->resize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
                viewportSize = {viewportPanelSize.x, viewportPanelSize.y};
            }

            // By passing this renderer ID, this gives us the ID of the texture that we want to render.
            uint32_t textureID = frameBuffer->getColorAttachmentRendererID(); // Getting color buffer from frame buffer
            ImGui::Image(reinterpret_cast<void *>(textureID), ImVec2{viewportSize.x, viewportSize.y}, ImVec2{0, 1}, ImVec2{1, 0});

            //! @note UI for drawing our drag/drop targets
            UI::UI_DragDropTarget("CONTENT_BROWSER_ITEM", [](const std::string& payloadID){
                // const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(payloadID.c_str());

                // std::filesystem::path filepath((const char*)payload);

            });
        });

        UI::UI_Toolbar("##toolbox", ImVec2(0, 2), ImVec2(0, 2), ImVec4(0, 0, 0, 0), (ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse), [this](){
            
            auto& color = ImGui::GetStyle().Colors;

            auto& buttonHovered = color[ImGuiCol_ButtonHovered];
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));

            auto& buttonActive = color[ImGuiCol_ButtonActive];
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

            bool toolbarEnabled = (bool)currentScene;

            ImVec4 tintColor = {1, 1, 1, 1};

            if(!toolbarEnabled){
                tintColor.w = 0.5f;
            }

            float size = ImGui::GetWindowHeight() - 4.0f;
            ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));

            ImGui::SameLine((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));

            //! @note Fetches icon play button image
            Ref<Texture2D> icon = (currentSceneState == SceneState::EDIT) ? playIcon : stopIcon;

            if(ImGui::ImageButton((ImTextureID)icon->GetRendererID(), ImVec2{size, size}, ImVec2(0, 0), ImVec2(1, 1))){
                if(currentSceneState == SceneState::EDIT){
                    OnScenePlay();
                }
                else if(currentSceneState == SceneState::PLAY){
                    OnSceneEdit();
                }
            }

            ImGui::PopStyleVar(2);
            ImGui::PopStyleColor(3);
        });

        ImGui::End();
    }




    //! @note Scene Serialization logic
    void EditorLayer::CreateScene(){
        currentScene = CreateRef<Scene>();
        currentScene->onViewportResize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
        sceneHeirarchy.SetNewScene(currentScene);
    }

    void EditorLayer::LoadScene(const std::string& path){
        std::string filepath = FileDialogs::openFile("Engine3D (*.engine)\0*.engine");
        if(currentSceneState == SceneState::PLAY)
            OnSceneEdit();

        Ref<Scene> scene = CreateRef<Scene>();
        SceneSerializer serializer(scene);
        if(serializer.deserialize(path)){
            editorScene = scene;
            editorScene->onViewportResize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
            sceneHeirarchy.SetNewScene(editorScene);
            currentScene = editorScene;
        }
    }

    void EditorLayer::SaveScene(){}


    //! @note Simulation-related logic
    void EditorLayer::OnScenePlay(){
        currentSceneState = SceneState::PLAY;
    }

    void EditorLayer::OnSceneEdit(){
        currentSceneState = SceneState::EDIT;
    }
};