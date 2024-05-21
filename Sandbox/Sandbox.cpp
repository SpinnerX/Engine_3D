#include <Engine3D/Core/Application.h>
#include <Engine3D/Core/EntryPoint.h>
#include "EditorLayer.h"

namespace Engine3D{
class UIApplication : public Application{
public:
    UIApplication(ApplicationCommandLineArgs args) : Application("UI Editor", args) {
        pushLayer(new EditorLayer());
    }

    ~UIApplication();
};

Application* CreateApplication(ApplicationCommandLineArgs args){
    return new UIApplication(args);
}
};
