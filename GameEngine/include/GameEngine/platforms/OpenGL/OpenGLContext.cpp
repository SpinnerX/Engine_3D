#include <GLFW/glfw3.h>
#include <GameEngine/platforms/OpenGL/OpenGLContext.h>

namespace RendererEngine{
	OpenGLContext::OpenGLContext(GLFWwindow* other) : _window(other) {
		render_core_assert(_window, "Window was nullptr!");
	}

	void OpenGLContext::Init(){
		RENDER_PROFILE_FUNCTION();

		glfwMakeContextCurrent(_window);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		render_core_assert(status, "Failed to initialize Glad!");

		coreLogInfo("OpenGL Info:");
		coreLogInfo("  Vendor: {0}", (const char *)glGetString(GL_VENDOR));
		coreLogInfo("  Renderer: {0}", (const char *)glGetString(GL_RENDERER));
		coreLogInfo("  Version: {0}", (const char *)glGetString(GL_VERSION));
	}

	void OpenGLContext::swapBuffers(){
		RENDER_PROFILE_FUNCTION();

		glfwSwapBuffers(_window);
	}

};
