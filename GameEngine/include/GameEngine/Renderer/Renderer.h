#pragma once
#include <GameEngine/Renderer/RenderCommand.h>

namespace RendererEngine{
    
    // Renderer
    // - The renderer is what allows for bind and unbinding
    // - Only Renderer abstracts away the bind and unbinding functions.
    class Renderer{
    public:
        static void beginScene(); // TODO: 

        static void endScene();

        // Submitted into a renderer queue
        // Then is evaluated probably in a different thread and will get rendered
        static void submit(const std::shared_ptr<VertexArray>& vertexArray);

        inline static RendererAPI::API getAPI() { return RendererAPI::getAPI(); }

    private:
        // static RendererAPI::API _rendererAPI;
    };
};