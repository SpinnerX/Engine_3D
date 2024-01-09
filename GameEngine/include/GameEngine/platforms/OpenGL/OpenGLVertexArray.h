#include <GameEngine/Renderer/VertexArray.h>


namespace RendererEngine{
    // VertexArray that is renderer API-specific to OpenGL
    class OpenGLVertexArray : public VertexArray{
    public:
        OpenGLVertexArray();
        virtual ~OpenGLVertexArray();

        virtual void bind() const override;
        virtual void unbind() const override; // NOTE: In vertex arrays there are use cases where you may want to unbind your data.

        virtual void addVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
        virtual void setIndexBuffer(const Ref<IndexBuffer>& vertexBuffer) override;

        virtual const std::vector<Ref<VertexBuffer>>& getVertexBuffer() const override { return _vertexBuffers; }
        virtual const Ref<IndexBuffer>& getIndexBuffer() const override { return _indexBuffer; }

    private:
        uint32_t _rendererID;
        std::vector<Ref<VertexBuffer>> _vertexBuffers; // mimicking how opengl does this, but on the CPU
        Ref<IndexBuffer> _indexBuffer;
    };
};


