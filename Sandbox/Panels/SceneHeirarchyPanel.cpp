#include <Engine3D/Engine3DPrecompiledHeader.h>
#include <Engine3D/Scene2D/Components.h>
#include <imgui/imgui.h>
#include "SceneHeirarchyPanel.h"
#include "UI/UI.h"


namespace Engine3D{
    extern const std::filesystem::path _assetPath = "Resources";

    SceneHeirachyPanel::SceneHeirachyPanel(const Ref<Scene>& ctx) : currentSceneContext(ctx) {}

    void SceneHeirachyPanel::SetNewScene(const Ref<Scene>& scene){
        currentSceneContext = scene;
        currentSelectedEntity = {};
    }

    void SceneHeirachyPanel::SetSelectedNewEntity(const Entity& entity){
        currentSelectedEntity = entity;
    }

    void SceneHeirachyPanel::OnUIRender(){
        //! @note Drawing our entity nodes through ImGui
        UI::UI_CreateWidget("SceneHeirarchy", [this](auto entityID){
            if(currentSceneContext){
                currentSceneContext->_registry.each([&](){
                    Entity entity = {entityID, currentSceneContext.get() };
                    DrawEntityNode(entity);
                });


                //! @note Deselecting entities node tab
                if(ImGui::IsMouseDown(0) and UI::IsWindowHovered()){
                    currentSceneContext = {};
                }

                UI::UI_PopupContextWindow(0, 1, [this](){
                    if(ImGui::MenuItem("Create Emtpy Entity")){
                        currentSceneContext->createEntity("Empty Entity");
                    }
                });

                // ImGui::Begin();
                UI::UI_CreateWidget("Properties", [this](){
                    if(currentSelectedEntity){
                        DrawComponents(currentSelectedEntity);
                    }
                });
            }
        });
    }

    void SceneHeirachyPanel::DrawEntityNode(Entity& entity){

    }

    void SceneHeirachyPanel::DrawComponents(Entity& entity){
        if(entity.HasComponent<TagComponent>()){
            auto& tag = entity.GetComponent<TagComponent>().tag;

            char buffer[256];

            memset(buffer, 0, sizeof(buffer));
            strcpy(buffer, tag.c_str());

            UI::UI_InputText("##Tag", entity);
            ImGui::SameLine();
            ImGui::PushItemWidth(-1);
        }

        if(ImGui::Button("Add Component"))
            ImGui::OpenPopup("Add Component");

        // if(ImGui::BeginPopup("Add Component")){}

        ImGui::SameLine();
        ImGui::PushItemWidth(-1);
        UI::UI_Popup("Add Component", [this](){
           if(!currentSelectedEntity.HasComponent<SpriteRendererComponent>()){
                if(ImGui::MenuItem("Camera")){
                    currentSelectedEntity.AddComponent<CameraComponent>();
                    ImGui::CloseCurrentPopup();
                }
           }

           if(!currentSelectedEntity.HasComponent<SpriteRendererComponent>()){
                if(ImGui::MenuItem("Sprite Renderer")){
                    currentSelectedEntity.AddComponent<SpriteRendererComponent>();
                    ImGui::CloseCurrentPopup();
                }
           }

           if(!currentSelectedEntity.HasComponent<CircleRendererComponent>()){
                if(ImGui::MenuItem("Circle Renderer")){
                    currentSelectedEntity.AddComponent<CircleRendererComponent>();
                    ImGui::CloseCurrentPopup();
                }
           }

           if(!currentSelectedEntity.HasComponent<RigidBody2DComponent>()){
                if(ImGui::MenuItem("Rigidbody 2D")){
                    currentSelectedEntity.AddComponent<RigidBody2DComponent>();
                    ImGui::CloseCurrentPopup();
                }
           }

           if(!currentSelectedEntity.HasComponent<BoxCollider2DComponent>()){
                if(ImGui::MenuItem("Box Collider 2D")){
                    currentSelectedEntity.AddComponent<BoxCollider2DComponent>();
                    ImGui::CloseCurrentPopup();
                }
           }


        });

        ImGui::PopItemWidth();

        UI::UI_DrawComponents<TransformComponent>("Transform", entity, [](auto& component){
			// Creating our headers.
			glm::vec3 rotation = glm::degrees(component.rotation);
			UI::UI_DrawVec3Controller("Translation", component.translation);
			UI::UI_DrawVec3Controller("Rotation", rotation);
			component.rotation = glm::radians(rotation);
			UI::UI_DrawVec3Controller("Scale", component.scale, 1.0f);
		});

		// @note Setting up camera component
		UI::UI_DrawComponents<CameraComponent>("Camera Component", entity, [](auto& component){
			auto& camera = component.camera;
			ImGui::Checkbox("Primary", &component.isPrimary);

			const char* projectionTypeStrings[] = {"Perspective", "Orthographic"};
			const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.getProjectionType()];
			
			// @note if BeginCombo has started.
			if(ImGui::BeginCombo("Projection", currentProjectionTypeString)){
				// @note seeing what currently selected projection type is. 
				for(int i = 0; i < 2; i++){
					// @note handling if the projection type selected is valid and selected
					bool isSelected = (currentProjectionTypeString == projectionTypeStrings[i]);
					if(ImGui::Selectable(projectionTypeStrings[i], isSelected)){
						currentProjectionTypeString = projectionTypeStrings[i];
						camera.setProjectionType((SceneCamera::ProjectionType)i);
					}

					// @note checking if already selected then setting the default focus.
					if(isSelected){
						ImGui::SetItemDefaultFocus();
					}
				}

				ImGui::EndCombo();
			}

			// @note checking if the camera component is orthographic.
			if(component.camera.getProjectionType() == SceneCamera::ProjectionType::Perspective){
				float verticalFov = glm::degrees(camera.getPerspectiveVerticalFOV());

				if(ImGui::DragFloat("FOV", &verticalFov))
					camera.setPerspectiveVerticalFOV(glm::radians(verticalFov));

				float perspNearClip = camera.getPerspectiveNearClip();
				if(ImGui::DragFloat("Near", &perspNearClip))
					camera.setPerspectiveNearClip(perspNearClip);
				
				float perspFarClip = camera.getPerspectiveFarClip();
				if(ImGui::DragFloat("Far", &perspFarClip))
					camera.setPerspectiveFarClip(perspFarClip);
			}
			
			if(component.camera.getProjectionType() == SceneCamera::ProjectionType::Orthographic){
				float orthoSize = camera.getOrthographicSize();

				if(ImGui::DragFloat("Size", &orthoSize))
					camera.setOrthographicSize(orthoSize);

				float nearClip = camera.getOrthoNearClip();
				if(ImGui::DragFloat("Near", &nearClip))
					camera.setOrthoNearClip(nearClip);
				
				float farClip = camera.getOrthoFarClip();
				if(ImGui::DragFloat("Far", &farClip))
					camera.setOrthoNearClip(farClip);
				ImGui::Checkbox("Fixed Aspect Ratio", &component.fixedAspectRatio);
			}
		});
		
		// @note setting up Sprite Renderer Component.
		UI::UI_DrawComponents<SpriteRendererComponent>("Sprite Renderer", entity, [](auto& component){
				ImGui::ColorEdit4("Color", glm::value_ptr(component.color));
				// Being able to drag in a texture 
				// @note using a button that is going to be the target for texture
				ImGui::Button("Texture", ImVec2(100.0f, 0.0f));

				// @note getting the texture here
				// @note by dragging/dropping those texture
				// @note for loading a texture via drag/drop
				if(ImGui::BeginDragDropTarget()){
					if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")){
						// unsigned char* filepath = (unsigned char*)payload->Data;

						if(payload->Data == nullptr) coreLogWarn("Data payload was invalid for dropping this texture image!");
						std::string filepath((const char*)payload->Data);
						std::filesystem::path texturePath = std::filesystem::path(_assetPath) / filepath;
						Ref<Texture2D> createdTexture = Texture2D::Create(texturePath);
						// component.texture = Texture2D::Create(texturePath);

						if(createdTexture->IsLoaded()){
							component.texture = createdTexture;
						}
						else{
							coreLogWarn("Could not load dragged image to create texture!");
						}
						// component.texture = 
					}

					ImGui::EndDragDropTarget();
				}

				// @note Doing Texture stuff
				ImGui::DragFloat("Tiling Factor", &component.tilingFactor, 0.1f, 0.0f, 100.0f);
		});

		UI::UI_DrawComponents<CircleRendererComponent>("Circle Renderer", entity, [](auto& component){
				// ImGui::ColorEdit4("Color", glm::value_ptr(component.color));
				// ImGui::DragFloat("Thickness", &component.thickness, 0.025f, 0.0f, 1.0f);
				ImGui::DragFloat("Fade", &component.fade, 0.00025f, 0.0f, 1.0f);
		});

		UI::UI_DrawComponents<RigidBody2DComponent>("Rigidbody 2D", entity, [](auto& component){
			const char* bodyTypeString[] = { "Static", "Dynamic", "Kinematic" };
			const char* currentBodyStringType = bodyTypeString[(int)component.type];

			if(ImGui::BeginCombo("Body Type", currentBodyStringType)){
				for(int i = 0; i < 2; i++){
					bool isSelected = currentBodyStringType == bodyTypeString[i];

					if(ImGui::Selectable(bodyTypeString[i], isSelected)){
						currentBodyStringType = bodyTypeString[i];
						component.type = (RigidBody2DComponent::BodyType)i;
					}

					if(isSelected)
						ImGui::SetItemDefaultFocus();

				}
				
				ImGui::EndCombo();
			}

			/* coreLogWarn("Failure Detected Debug #"); */

			ImGui::Checkbox("Fixed Rotation", &component.hasFixedRotation);
		});


		UI::UI_DrawComponents<BoxCollider2DComponent>("Box Collider 2D", entity, [](auto& component){
			ImGui::DragFloat("Offset", glm::value_ptr(component.offset));
			ImGui::DragFloat("Size", glm::value_ptr(component.size));
			ImGui::DragFloat("Density", &component.density);
			ImGui::DragFloat("Friction", &component.friction, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Restitution", &component.restitution, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Restitution Threshold", &component.restitutionThreshold, 0.01f, 0.1f, 1.0f);
		});



    }
};