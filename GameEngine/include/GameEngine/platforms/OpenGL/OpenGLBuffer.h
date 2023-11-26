#pragma once
#include <GameEngine/Renderer/Buffer.h>

namespace RendererEngine{
    // VertexBuffer class for OpenGL specific
    class OpenGLVertexBuffer : public VertexBuffer{
    public:
        OpenGLVertexBuffer(float* vertices, uint32_t size);
        virtual ~OpenGLVertexBuffer();

        virtual void bind() const override;
        virtual void unbind() const override;

        virtual void setLayout(const BufferLayout& layout) override {
            _layout = layout;
        }

        virtual const BufferLayout& getLayout() const override {
            return _layout;
        }

    private:
        uint32_t _rendererID; // Should be stored in the actual implementation and not the actual vertex buffer
        BufferLayout _layout;
    };


    // IndexBuffer class for OpenGL specific
    // Creating index buffer
    // element buffer and index buffers are the same thing people referred to them as element buffer
    // - Buffer of indices of index into this buffer
    // - tell what order of where to draw these vertices.
    // - Kind of like into an index into an array.
    class OpenGLIndexBuffer : public IndexBuffer{
    public:
        OpenGLIndexBuffer(uint32_t* indices, uint32_t size);
        virtual ~OpenGLIndexBuffer();

        virtual void bind() const override;
        virtual void unbind() const override;

        // This will tell us how many indices are in this index buffer.
        // To differentiate getCount() means in elements and NOT in bytes.
        // Meaning multiple by the size
        // If the size is in bytes then 
        virtual uint32_t getCount() const override { return _count; }

    private:
        uint32_t _rendererID; // Should be stored in the actual implementation and not the actual vertex buffer
        uint32_t _count;
    };
};