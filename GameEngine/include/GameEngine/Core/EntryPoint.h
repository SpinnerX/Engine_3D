#pragma once
#include <GameEngine/Core/core.h>
// if we are on the Windows OS (since windows uses dll files)

// We move this entry point hhere
// How this is able to happen is because with __attribute__((visibility("default")))
// that allows us to create different entry points when creating game engines, and stuff

#ifdef RENDER_PLATFORM_WINDOWS
  extern RendererEngine::Application* RendererEngine::CreateApplication();

  int main(int argc, char** argv){

      auto app = RendererEngine::CreateApplication();
      app->Run();
      delete app;
  }
#else
  extern RendererEngine::Application* RendererEngine::CreateApplication();

  int main(int argc, char** argv){
    RendererEngine::EngineLogger::Init();
	RENDER_PROFILE_BEGIN_SESSION("Startup", "EngineProfile-Startup.json");
    auto app = RendererEngine::CreateApplication();
	RENDER_PROFILE_END_SESSION();

	RENDER_PROFILE_BEGIN_SESSION("Runtime", "EngineProfile-Runtime.json");
    app->Run();
	RENDER_PROFILE_END_SESSION();

	RENDER_PROFILE_BEGIN_SESSION("Startup", "EngineProfile-Shutdown.json");
    delete app;
	RENDER_PROFILE_END_SESSION();
}
#endif
