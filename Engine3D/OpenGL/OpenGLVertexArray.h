#include <Engine3D/Graphics/VertexArray.h>


namespace Engine3D{
    // VertexArray that is renderer API-specific to OpenGL
    class OpenGLVertexArray : public VertexArray{
    public:
        OpenGLVertexArray();
        virtual ~OpenGLVertexArray();

        virtual void Bind() const override;
        virtual void Unbind() const override; // NOTE: In vertex arrays there are use cases where you may want to unbind your data.

        virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
        virtual void SetIndexBuffer(const Ref<IndexBuffer>& vertexBuffer) override;

        virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffer() const override { return _vertexBuffers; }
        virtual const Ref<IndexBuffer>& GetIndexBuffer() const override { return _indexBuffer; }

    private:
        uint32_t _rendererID;
		uint32_t index = 0;
        std::vector<Ref<VertexBuffer>> _vertexBuffers; // mimicking how opengl does this, but on the CPU
        Ref<IndexBuffer> _indexBuffer;
    };
};


