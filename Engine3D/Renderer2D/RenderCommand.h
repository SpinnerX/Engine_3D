#pragma once
#include <Engine3D/Graphics/RendererAPI.h>

namespace Engine3D{
    // RendererCommand 
    // - Will contain a class that will handle rendering command calls to render objects
    // - One byte memory leak, where it points to a 1 byte memory adddress not the ptr itself (if it is)
    // - 1 byte of memory
    class RendererCommand{
    public:
        
        // Initializing our rendering draw command
        // Also initializing our rendering API's
        inline static void init(){
            _rendererAPI->init();
        }

		inline static void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height){
			_rendererAPI->setViewport(x, y, width, height);
		}

        inline static void setClearColor(const glm::vec4& color) {
            _rendererAPI->setClearColor(color);
        }

        inline static void clear() {
            _rendererAPI->clear();
        }

        // A static wrapper for actual implementation
        // Going to dispatch commands from drawIndexed
        inline static void drawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0){
            _rendererAPI->drawIndexed(vertexArray, indexCount);
        }

    private:
        static Scope<RendererAPI> _rendererAPI; // ptr should last throughout our entire lifetime of our whole application
    };
};
