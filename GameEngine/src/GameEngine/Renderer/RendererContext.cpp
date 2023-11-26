#include <GameEngine/Renderer/RendererContext.h>
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <GL/gl.h>

namespace RendererEngine{
    RendererContext::RendererContext(GLFWwindow* windowHandler) : _window(windowHandler) {
        render_core_assert(windowHandler, "Window handle is nullptr!!!");
    }

    void RendererContext::Init(){
        glfwMakeContextCurrent(_window); // Basically making this current window our context window
        gladLoadGL(); // We need this before we use gladLoadGLLoader
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

        render_core_assert(status, "Failed to initialized glad!");
    }

    void RendererContext::swapBuffers(){
        glfwSwapBuffers(_window);
    }
};