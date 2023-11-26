#include <GameEnginePrecompiledHeader.h>
#include <GameEngine/Renderer/Buffer.h>
#include <GameEngine/Renderer/Renderer.h>
#include <GameEngine/platforms/OpenGL/OpenGLBuffer.h>

namespace RendererEngine{

    // Now use these functions to decide which renderer API's that we want to use
    // - such as which class to instantiate and utilize.
    VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size){
        switch (RendererAPI::getAPI()){
        case RendererAPI::API::None:
            render_core_assert(false, "RenderAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return new OpenGLVertexBuffer(vertices, size);
        }
        render_core_assert(false, "Unknown renderer API");
        return nullptr;
    }

    IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size){
        switch (RendererAPI::getAPI()){
        case RendererAPI::API::None:
            render_core_assert(false, "RenderAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return new OpenGLIndexBuffer(indices, size);
        }
        render_core_assert(false, "Unknown renderer API");
        return nullptr;
    }
};