#include <Engine3D/Engine3DPrecompiledHeader.h>
#include "UI.h"

namespace Engine3D{
    bool UI::IsViewortHovered(int flag) {
        return ImGui::IsWindowHovered(flag);
    }

    void UI::ToolBar(){}

    ImVec2 UI::GetViewportSize(){
        return ImGui::GetContentRegionAvail();
    }
};