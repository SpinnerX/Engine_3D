#pragma once
#include <Engine3D/Core/core.h>
#include <Engine3D/Core/Layer.h>
#include <Engine3D/Event/Event.h>
#include <Engine3D/Graphics/Texture.h>
#include <Engine3D/Scene2D/Entity.h>
#include <Engine3D/Scene2D/Scene.h>
#include <Engine3D/Graphics/Framebuffer.h>
#include <Engine3D/Renderer2D/EditorCamera.h>
#include "Panels/SceneHeirarchyPanel.h"

namespace Engine3D{
    class EditorLayer : public Layer{
    public:
        EditorLayer();

        void OnAttach() override;

        void OnDetach() override;

        void OnUpdate(Timestep ts) override;

        void OnEvent(Event& event) override;

        void OnUIRender() override;

    private:
        // Serialization/Deserialization Handlers

        //! @note Creating a new scene to work with.
        void CreateScene();
        
        //! @note Loading already-serialized scene
        void LoadScene(const std::string& path);

        //! @note Save As
        void SaveScene();

        //! @note Simulation logic
        //! @param OnScenePlay
        //! @note Physics simulation runtime.
        //! @note Will start our simulation
        //! @note Used for playing animations

        //! @param OnSceneEdit
        //! @note Using our editor runtime.
        //! @note Used for stopping our simulation, and entering into editor mode.
        void OnScenePlay();
        void OnSceneEdit();

    private:
        enum class SceneState : uint8_t {
            EDIT,
            PLAY,
            STOP
        };

        EditorCamera editorCamera;
        Ref<FrameBuffer> frameBuffer;

        SceneHeirachyPanel sceneHeirarchy;

        bool isViewportFocused = false;
        bool isViewportHovered = false;

        // Ref<Texture2D> icon;
        Ref<Texture2D> playIcon;
        Ref<Texture2D> stopIcon;

        //! @note TODO --- Scene Graph to control what our current scene is
        //! @note Right now we manually handle how our scenes are copied or duplicated
        Ref<Scene> currentScene;
        Ref<Scene> editorScene;
        Ref<Scene> runtimeScene;
        Entity hoveredEdity;

        glm::vec2 viewportSize;
        glm::vec2 viewportBound[2];

        //! @note Keeping track what our scenes current state is in. (used for physics simulation)
        SceneState currentSceneState = SceneState::EDIT;
    };
};