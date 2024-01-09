#pragma once
#include <GameEngine/Renderer/GraphicsContext.h>

namespace RendererEngine{
	// Generally when we are creating an OpenGLContext (RendererContext)
    // - We need some kind of hahndler in thhe window to control things
    // - Taking in a GLFWwindow thhrough the constructor
    class OpenGLContext : public GraphicsContext{
    public:
        OpenGLContext(GLFWwindow* window);

        virtual void Init() override;
        virtual void swapBuffers() override;

    private:
      GLFWwindow* _window;
    };
};
