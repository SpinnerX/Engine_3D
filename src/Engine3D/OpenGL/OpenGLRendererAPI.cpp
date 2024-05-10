#include <Engine3D/Engine3DPrecompiledHeader.h>
#include <OpenGL/OpenGLRendererAPI.h>

namespace Engine3D{

    void OpenGLRendererAPI::init() {
		RENDER_PROFILE_FUNCTION();

        // What allows for testing
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
    }

    void OpenGLRendererAPI::setClearColor(const glm::vec4& color) {
        glClearColor(color.r, color.g, color.b, color.a);
    }

	void OpenGLRendererAPI::setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height){
		glViewport(x, y, width, height);
	}

    void OpenGLRendererAPI::clear() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLRendererAPI::drawIndexed(const Ref<VertexArray>& vertexArr, uint32_t indexCount) {
		uint32_t count = indexCount ? vertexArr->GetIndexBuffer()->getCount() : indexCount;
        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
		glBindTexture(GL_TEXTURE_2D, 0);
    }

};
