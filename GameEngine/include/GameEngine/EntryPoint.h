#pragma once
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
    auto app = RendererEngine::CreateApplication();
    app->Run();
    delete app;
}
#endif