#include <Engine3D/Engine3DPrecompiledHeader.h>
#include <OpenGL/OpenGLBuffer.h>

namespace Engine3D{
    /// -------------------------------------------------
    /// --------------- [ Vertex Buffer] ----------------
    /// -------------------------------------------------
    OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size) {
		ENGINE_PROFILE_FUNCTION();

        glGenBuffers(1, &_rendererID);
        // glCreateBuffers(1, &_rendererID); // For some reason does not work but glGenBuffers does
        glBindBuffer(GL_ARRAY_BUFFER, _rendererID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    }
	
	OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size){
		ENGINE_PROFILE_FUNCTION();

		glGenBuffers(1, &_rendererID);
		glBindBuffer(GL_ARRAY_BUFFER, _rendererID);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW); // Going to work because we know the data is going to be dynamic
	}

    OpenGLVertexBuffer::~OpenGLVertexBuffer() {
		ENGINE_PROFILE_FUNCTION();

        glDeleteBuffers(1, &_rendererID);
    }

    void OpenGLVertexBuffer::Bind() const {
		ENGINE_PROFILE_FUNCTION();

        glBindBuffer(GL_ARRAY_BUFFER, _rendererID);
    }

    void OpenGLVertexBuffer::Unbind() const {
		ENGINE_PROFILE_FUNCTION();

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
	void OpenGLVertexBuffer::SetData(const void* data, uint32_t size){
		glBindBuffer(GL_ARRAY_BUFFER, _rendererID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

    /// -------------------------------------------------
    /// ---------------- [ Index Buffer] ----------------
    /// -------------------------------------------------

    OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count) : _count(count) {
		ENGINE_PROFILE_FUNCTION();

        // NOTE: For some reason glCreateBuffers segfaults when used in OpenGLIndexBuffer and OpenGLVertexBuffers!
        glGenBuffers(1, &_rendererID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _rendererID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
    }

    OpenGLIndexBuffer::~OpenGLIndexBuffer(){
		ENGINE_PROFILE_FUNCTION();

        glDeleteBuffers(1, &_rendererID);
    }

    void OpenGLIndexBuffer::Bind() const{
		ENGINE_PROFILE_FUNCTION();

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _rendererID);
    }

    void OpenGLIndexBuffer::Unbind() const{
		ENGINE_PROFILE_FUNCTION();

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
};
