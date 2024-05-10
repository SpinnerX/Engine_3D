#include <Engine3D/Engine3DPrecompiledHeader.h>
#include <Engine3D/Renderer2D/Renderer.h>

namespace Engine3D{
	Scope<Renderer::SceneData> Renderer::_sceneData = CreateScope<Renderer::SceneData>();

    void Renderer::init(){
		RENDER_PROFILE_FUNCTION();

        RendererCommand::init();
		Renderer2D::Init(); // Need to initialize 2D renderer before anything.
    }
	
	void Renderer::onWindowResize(uint32_t w, uint32_t h){
		RendererCommand::setViewport(0, 0, w, h);
	}

    void Renderer::Begin(OrthographicCamera& camera){
        _sceneData->viewProjectionMatrix = camera.getViewProjectionMatrix();
    }

    void Renderer::End(){

    }

    void Renderer::submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform){
        shader->Bind();
        // std::dynamic_pointer_cast<OpenGLShader>(shader)->Set("u_ViewProjection", _sceneData->viewProjectionMatrix);
        // std::dynamic_pointer_cast<OpenGLShader>(shader)->Set("u_Transform", transform); // whenever we submit, def need to do this per object
        shader->Set("u_ViewProjection", _sceneData->viewProjectionMatrix);
        shader->Set("u_Transform", transform);

        vertexArray->Bind();
        RendererCommand::drawIndexed(vertexArray);
    }
};
