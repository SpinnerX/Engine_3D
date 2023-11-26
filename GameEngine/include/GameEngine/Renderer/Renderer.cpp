#include <GameEnginePrecompiledHeader.h>
#include <GameEngine/Renderer/Renderer.h>

namespace RendererEngine{

    void Renderer::beginScene(){

    }

    void Renderer::endScene(){

    }

    void Renderer::submit(const std::shared_ptr<VertexArray>& vertexArray){
        vertexArray->bind();
        RendererCommand::drawIndexed(vertexArray);
    }
};