#pragma once
#include <Engine3D/Core/core.h>
#include <Engine3D/Scene2D/Components.h>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <string>
#include <glm/glm.hpp>
#include <Engine3D/Scene2D/Entity.h>
#include <Engine3D/Core/EngineLogger.h>

namespace Engine3D{
    /**
     * @param UI
     * @note UI class for creating imgui UI-related widgets, and various other ui handlers
     * 
    */
    class UI{
    public:
        void ToolBar();

        static bool IsViewortHovered(int flag = 0);

        static ImVec2 GetViewportSize();

        static bool IsWindowFocused() { return ImGui::IsWindowFocused(); }
        static bool IsWindowHovered() { return ImGui::IsWindowHovered(); }

        // static bool BeginPopupContextWindow(const std::string& id, ImGuiMouseButton mb, bool over_items){
        //     return ImGui::BeginPopupContextWindow(id.c_str(), mb | (over_items ? 0 : ImGuiPopupFlags_NoOpenOverItems));
        // }


        /**
         * @brief 

         Usage API

        UI_CreateViewport([this](){
            // Do viewport stuff.....
        });

         * 
         * @tparam UFunction 
         * @param function 
         */
        template<typename UFunction>
        static void UI_CreateViewport(const UFunction&& function){
            ImGui::Begin("Viewport");

            function();

            ImGui::End();
        }

        template<typename UFunction>
        static void UI_CreateWidget(const std::string& id, const UFunction& function = UFunction()){
            if(ImGui::Begin(id.c_str())){

                ImGui::End();
            }
        }

        template<typename UFunction>
        static void UI_CreatePanel(const std::string& id, const UFunction&& function){
            if(ImGui::Begin(id.c_str())){
                function();
            }

            ImGui::End();
        }


        template<typename UFunction>
        static void UI_Menubar(const UFunction&& function){
            if(ImGui::BeginMenuBar()){
                function();
            }

            ImGui::EndMenuBar();
        }


        template<typename UFunction>
        static void UI_DragDropTarget(const std::string& payloadID = "Default Payload ID", const UFunction&& function = UFunction()){
            if(ImGui::BeginDragDropTarget()){
                function(payloadID);
                ImGui::EndDragDropTarget();
            }
        }


        //! @note Inputting our text and returns back our buffer that we typed to 
        /*
        @example -- usage

        Entitiy entity;
        UI::UI_InputText("##Tag", entity);
        
        */
        static void UI_InputText(const std::string nameID, Entity& entity){
            auto& tag = entity.GetComponent<TagComponent>().tag;
            // std::string tag = "Demo Testing Input Text! --- Replace with actual TagComponent value!";
            if(tag.empty()){
                coreLogWarn("TagComponent.c_str() == empty()!");
                return;
            }

            char buffer[256];
            memset(buffer, 0, sizeof(buffer));
            strcpy(buffer, tag.c_str());

            if(ImGui::InputText(nameID.c_str(), buffer, sizeof(buffer))){
                tag = std::string(buffer);
            }
        }

        template<typename UFunction>
        static void UI_Popup(const std::string& id, const UFunction& function = UFunction()){
            if(ImGui::BeginPopup(id.c_str())){
                function();
                ImGui::EndPopup();
            }
        }
        

        // template<typename UFunction>
        // static void UI_Popup(const std::string& nameID, )

        template<typename UFunction>
        static void UI_Toolbar(const std::string& id, const ImVec2& winStylePadIdx, const ImVec2& itemInnerSpacing, const ImVec4& buttonColor, ImGuiWindowFlags flags, const UFunction& function){
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, winStylePadIdx); // @note ImVec making button not touch bottom
            ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, itemInnerSpacing);
            ImGui::PushStyleColor(ImGuiCol_Button, buttonColor);

            if(ImGui::Begin(id.c_str(), nullptr, flags)){
                function();

                ImGui::End();
            }
        }

        template<typename UFunction>
        static void UI_PopupContextWindow(const int str_id, const int mb, const UFunction& function){
            if(BeginPopupContextWindow(0, 1, false)){
                function();

                ImGui::EndPopup();
            }
        }

        // static void UI_Button(const ImVec2& size, const ImVec4& buttonColor, const ImVec4& btnHoveredColor, const ImVec4& btnActiveColor, ImFont* font){

        // }
        /**
        *
        * @param label - imgui requires that is inputted a unique-id for uniqueness among imgui widgets.
        * @param values - transform/rotation/scaling values
        * @param resetVal - values to reset translation back to zero
        * @param columnWidth - width of our given labels.
        * */
        static void UI_DrawVec3Controller(const std::string& nameID, glm::vec3& values, float resetValues=0.0f, float colWidth=100.0f){
            ImGuiIO& io = ImGui::GetIO();
            auto boldFont = io.Fonts->Fonts[0]; // @note getting default font from vector<ImFont*>

            ImGui::PushID(nameID.c_str()); // Meaning that specific group of ID's are pushed have a unique label

            ImGui::Columns(2); // two columns, one for the labels
            
            ImGui::SetColumnWidth(0, colWidth);
            ImGui::Text("%s", nameID.c_str());
            ImGui::NextColumn();
            
            ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 0});
            
            // This calculates the height 
            float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
            ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.8, 0.1f, 0.15f, 1.0f});
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.9f, 0.2f, 0.2f, 1.0f});
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.8, 0.1f, 0.15f, 1.0f});

            // Setting up the buttons
            
            // Setting up for the Y button
            ImGui::PushFont(boldFont);
            if(ImGui::Button("X", buttonSize))
                values.x = resetValues;

            ImGui::PopFont();
            ImGui::PopStyleColor(3);

            ImGui::SameLine();
            ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
            ImGui::PopItemWidth();
            ImGui::SameLine();


            // Setting up for the Y button
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.2, 0.7f, 0.2f, 1.0f});
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.3f, 0.8f, 0.3f, 1.0f});
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.2, 0.1f, 0.2f, 1.0f});

            ImGui::PushFont(boldFont);
            if(ImGui::Button("Y", buttonSize))
                values.y = resetValues;

            ImGui::PopFont();
            ImGui::PopStyleColor(3);
            ImGui::SameLine();
            ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
            ImGui::PopItemWidth();
            ImGui::SameLine();

            // Setting up for the Z button
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.1, 0.25f, 0.8f, 1.0f});
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.2f, 0.35f, 0.9f, 1.0f});
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.8, 0.1f, 0.15f, 1.0f});
            ImGui::PushFont(boldFont);
            if(ImGui::Button("Z", buttonSize))
                values.z = resetValues;

            ImGui::PopFont();
            ImGui::PopStyleColor(3);
            ImGui::SameLine();
            ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
            ImGui::PopItemWidth();
            
            ImGui::PopStyleVar();

            ImGui::Columns(1);

            ImGui::PopID();
        }

        //! @note When you want to push a bunch of widgets to your UI
        //! @note ImGui requires that you need to specify a unique-id to uniquely differentiate them.
        template<typename UFunction>
        static void UI_Group(const std::string& id, const glm::vec3& values, float resetValues=0.0f, float colWidth=100.0f, const UFunction& function = UFunction()){
            ImGui::PushID(id.c_str());

            function();

            ImGui::PopID();
        }

        /**
        * @param T is the component type we are passing
        * @param UIFuncion is the lambda function.
        * @note Used for rendering our entity nodes to our UI
        **/
        template<typename T, typename UIFunction>
        static void UI_DrawComponents(const std::string& name, Entity entity, UIFunction function){
            const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

            if(entity.HasComponent<T>()){
                auto& component = entity.GetComponent<T>();
                ImVec2 contentRegion = ImGui::GetContentRegionAvail();

                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
                
                float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f; // @note calculating height of button
                ImGui::Separator();

                bool opened = ImGui::TreeNodeEx((void *)typeid(T).hash_code(), treeNodeFlags, "%s", name.c_str());
                ImGui::PopStyleVar();
                
                // @note seeing what the most available width are.
                // @note using line height takes account for the scroll bar.
                // @note if scrollbar appears then moving everthing to the left, so there are no overlapping with the scrollbar.
                ImGui::SameLine(contentRegion.x  - lineHeight * 0.5f);
                
                if(ImGui::Button("+", ImVec2{lineHeight, lineHeight})){
                    ImGui::OpenPopup("ComponentSettings");
                }

                
                // @note contain menu items for the components.
                bool isRemovedComponent = false; // @note for deferring when to delete component.
                if(ImGui::BeginPopup("ComponentSettings")){
                    if(ImGui::MenuItem("Remove Component"))
                        isRemovedComponent = true;

                    ImGui::EndPopup();
                }

                if(opened){
                    // uiFunction(component);
                    function(component);
                    ImGui::TreePop();
                }
                
                if(isRemovedComponent)
                    entity.RemoveComponent<T>();
            }
        }

    private:
        // Since BeginPopupContextWindow(const char*, ImGuiMouseButton, bool) is obsolete in ImGui, just recreated that function call through here.
        static bool BeginPopupContextWindow(const char* str_id, ImGuiMouseButton mb, bool over_items) {
            return ImGui::BeginPopupContextWindow(str_id, mb | (over_items ? 0 : ImGuiPopupFlags_NoOpenOverItems));
        }
    };
};