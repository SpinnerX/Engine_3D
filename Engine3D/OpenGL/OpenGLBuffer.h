#pragma once
#include <Engine3D/Graphics/Buffer.h>

namespace Engine3D{
    // VertexBuffer class for OpenGL specific
    class OpenGLVertexBuffer : public VertexBuffer{
    public:
        OpenGLVertexBuffer(float* vertices, uint32_t size);
		OpenGLVertexBuffer(uint32_t size);
        virtual ~OpenGLVertexBuffer();

        virtual void Bind() const override;
        virtual void Unbind() const override;
		
		virtual void SetData(const void* data, uint32_t size) override;

        virtual void SetLayout(const BufferLayout& layout) override {
            _layout = layout;
        }

        virtual const BufferLayout& GetLayout() const override {
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

        virtual void Bind() const override;
        virtual void Unbind() const override;

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
