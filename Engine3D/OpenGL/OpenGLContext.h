#pragma once
#include <Engine3D/Graphics/GraphicsContext.h>
#include <glad/glad.h>
#include  <GLFW/glfw3.h>

namespace Engine3D{
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
