#include <Engine3D/Engine3DPrecompiledHeader.h>
#include <Engine3D/Core/EntryPoint.h>
// if we are on the Windows OS (since windows uses dll files)

// We move this entry point hhere
// How this is able to happen is because with __attribute__((visibility("default")))
// that allows us to create different entry points when creating game engines, and stuff

#ifdef ENGINE_PLATFORM_WINDOWS
  extern Engine3D::Application* Engine3D::CreateApplication();

  int main(int argc, char** argv){

      auto app = Engine3D::CreateApplication();
      app->Run();
      delete app;
  }
#else
  extern Engine3D::Application* Engine3D::CreateApplication(Engine3D::ApplicationCommandLineArgs args);

  int main(int argc, char** argv){
    Engine3D::EngineLogger::Init(); 
    auto app = Engine3D::CreateApplication({argc, argv});
    app->Run();
    delete app;
}
#endif
