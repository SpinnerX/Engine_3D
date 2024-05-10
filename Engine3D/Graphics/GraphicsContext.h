#pragma once

namespace Engine3D{
    // - Will be a kind of interface that will send things like swapbuffers, and Init().
    // - Utilized by RendererContext
    class GraphicsContext{
    public:
		virtual ~GraphicsContext() = default;
        virtual void Init() = 0; // To initialize the renderer
        virtual void swapBuffers() = 0; // Swapbuffers (doing swapchains)
    };
};
