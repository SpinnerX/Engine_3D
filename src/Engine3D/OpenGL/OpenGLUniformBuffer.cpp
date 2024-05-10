#include <Engine3D/Engine3DPrecompiledHeader.h>
#include <OpenGL/OpenGLUniformBuffer.h>

namespace Engine3D{
	
	OpenGLUniformBuffer::OpenGLUniformBuffer(uint32_t size, uint32_t binding){
		glGenBuffers(1, &_rendererID);
		glNamedBufferData(_rendererID, size, nullptr, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, binding, _rendererID);
	}

	OpenGLUniformBuffer::~OpenGLUniformBuffer(){
		glDeleteBuffers(1, &_rendererID);
	}

	void OpenGLUniformBuffer::setData(const void* data, uint32_t size, uint32_t offset){
		glNamedBufferSubData(_rendererID, offset, size, data);
	}
};
