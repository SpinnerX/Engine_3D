#include <GameEngine/Renderer/VertexArray.h>


namespace RendererEngine{
    // VertexArray that is renderer API-specific to OpenGL
    class OpenGLVertexArray : public VertexArray{
    public:
        OpenGLVertexArray();
        virtual ~OpenGLVertexArray();

        virtual void bind() const override;
        virtual void unbind() const override; // NOTE: In vertex arrays there are use cases where you may want to unbind your data.

        virtual void addVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
        virtual void setIndexBuffer(const std::shared_ptr<IndexBuffer>& vertexBuffer) override;

        virtual const std::vector<std::shared_ptr<VertexBuffer>>& getVertexBuffer() const override { return _vertexBuffers; }
        virtual const std::shared_ptr<IndexBuffer>& getIndexBuffer() const override { return _indexBuffer; }

    private:
        uint32_t _rendererID;
        std::vector<std::shared_ptr<VertexBuffer>> _vertexBuffers; // mimicking how opengl does this, but on the CPU
        std::shared_ptr<IndexBuffer> _indexBuffer;
    };
};


