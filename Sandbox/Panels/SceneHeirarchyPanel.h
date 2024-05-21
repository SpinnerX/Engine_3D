#pragma once
#include <Engine3D/Core/core.h>
#include <Engine3D/Scene2D/Scene.h>
#include <Engine3D/Scene2D/Entity.h>

namespace Engine3D{
    /**
     * @param SceneHeirarchyPanel
     * @note As of right now is how we manage our scenes
     * @note Can be used to show our scene graph in imgui
     */
    class SceneHeirachyPanel{
    public:
        SceneHeirachyPanel() = default;
        SceneHeirachyPanel(const Ref<Scene>& ctx);

        void SetNewScene(const Ref<Scene>& scene);

        void SetSelectedNewEntity(const Entity& entity);

        void OnUIRender();


    private:
        void DrawEntityNode(Entity& entity);
        void DrawComponents(Entity& entity);

    private:
        //! @note Used for keeping track of what is our current scene
        //! @note Our context of our scene
        Ref<Scene> currentSceneContext;

        //! @note Keeping track of our currently selected entity.
        Entity currentSelectedEntity;
    };
};