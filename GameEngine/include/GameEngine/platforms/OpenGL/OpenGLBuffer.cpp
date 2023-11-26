#include <GameEnginePrecompiledHeader.h>
#include <GameEngine/platforms/OpenGL/OpenGLBuffer.h>
#include <glad/glad.h>

namespace RendererEngine{
    /// -------------------------------------------------
    /// --------------- [ Vertex Buffer] ----------------
    /// -------------------------------------------------
    OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size) {
        glGenBuffers(1, &_rendererID);
        // glCreateBuffers(1, &_rendererID); // For some reason does not work but glGenBuffers does
        glBindBuffer(GL_ARRAY_BUFFER, _rendererID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    }

    OpenGLVertexBuffer::~OpenGLVertexBuffer() {
        glDeleteBuffers(1, &_rendererID);
    }

    void OpenGLVertexBuffer::bind() const {
        glBindBuffer(GL_ARRAY_BUFFER, _rendererID);
    }

    void OpenGLVertexBuffer::unbind() const {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    /// -------------------------------------------------
    /// ---------------- [ Index Buffer] ----------------
    /// -------------------------------------------------

    OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count) : _count(count) {
        // NOTE: For some reason glCreateBuffers segfaults when used in OpenGLIndexBuffer and OpenGLVertexBuffers!
        glGenBuffers(1, &_rendererID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _rendererID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
    }

    OpenGLIndexBuffer::~OpenGLIndexBuffer(){
        glDeleteBuffers(1, &_rendererID);
    }

    void OpenGLIndexBuffer::bind() const{
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _rendererID);
    }

    void OpenGLIndexBuffer::unbind() const{
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
};