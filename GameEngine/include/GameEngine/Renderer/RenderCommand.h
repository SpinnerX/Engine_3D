#pragma once
#include <GameEngine/Renderer/RendererAPI.h>

namespace RendererEngine{
    // RendererCommand 
    // - Will contain a class that will handle rendering command calls to render objects
    // - One byte memory leak, where it points to a 1 byte memory adddress not the ptr itself (if it is)
    // - 1 byte of memory
    class RendererCommand{
    public:

        inline static void setClearColor(const glm::vec4& color) {
            _rendererAPI->setClearColor(color);
        }

        inline static void clear() {
            _rendererAPI->clear();
        }

        // A static wrapper for actual implementation
        // Going to dispatch commands from drawIndexed
        inline static void drawIndexed(const std::shared_ptr<VertexArray>& vertexArray){
            _rendererAPI->drawIndexed(vertexArray);
        }

    private:
        static RendererAPI* _rendererAPI; // ptr should last throughout our entire lifetime of our whole application
    };
};