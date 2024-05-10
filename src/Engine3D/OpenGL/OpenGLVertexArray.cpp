#include <Engine3D/Engine3DPrecompiledHeader.h>
#include <OpenGL/OpenGLVertexArray.h>

namespace Engine3D{
    static GLenum shaderDatatTypeToOpenGlBaseTypeConversion(ShaderDataType type){
        switch (type){
            case ShaderDataType::None: 
                coreLogInfo("Switch Case: None");
                return 0;
            case ShaderDataType::Float:  return GL_FLOAT;
            case ShaderDataType::Float2: return GL_FLOAT;
            case ShaderDataType::Float3: return GL_FLOAT;
            case ShaderDataType::Float4: return GL_FLOAT;
            case ShaderDataType::Mat3:   return GL_FLOAT;
            case ShaderDataType::Mat4:   return GL_FLOAT;
            case ShaderDataType::Int:    return GL_INT;
            case ShaderDataType::Int2:   return GL_INT;
            case ShaderDataType::Int3:   return GL_INT;
            case ShaderDataType::Int4:   return GL_INT;
            case ShaderDataType::Bool:   return GL_BOOL;
        }

        // render_core_assert(false, "Unknown ShaderDataType!");
        assert(false);
        return 0;
    }

    OpenGLVertexArray::OpenGLVertexArray(){
		RENDER_PROFILE_FUNCTION();
		
        // glCreateVertexArrays(1, &_rendererID);
        glGenVertexArrays(1, &_rendererID);
    }

    OpenGLVertexArray::~OpenGLVertexArray(){
		RENDER_PROFILE_FUNCTION();

        glDeleteVertexArrays(1, &_rendererID);
    }

    void OpenGLVertexArray::Bind() const {
		RENDER_PROFILE_FUNCTION();

        glBindVertexArray(_rendererID);
    }

    void OpenGLVertexArray::Unbind() const {
		RENDER_PROFILE_FUNCTION();

        glBindVertexArray(0);
    }

    void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) {
        // render_core_assert(vertexBuffer->getLayout().getElements().size(), "Vertex buffer has no layout!");
        
        glBindVertexArray(_rendererID);

        vertexBuffer->Bind();

        /* uint32_t index = 0; */
        const auto& layout = vertexBuffer->GetLayout();
        for(const auto& element : layout){
			switch (element.type) {
			case Engine3D::ShaderDataType::Float:
			case Engine3D::ShaderDataType::Float2:
			case Engine3D::ShaderDataType::Float3:
			case Engine3D::ShaderDataType::Float4:
				{
                glEnableVertexAttribArray(index);
                glVertexAttribPointer(index,
                                        element.GetComponentCount(),
                                        shaderDatatTypeToOpenGlBaseTypeConversion(element.type),
                                        element.isNormalized ? GL_TRUE : GL_FALSE,
                                        vertexBuffer->GetLayout().getStride(),
                                        (const void*)element.offset); // provind a vertex a shader
                index++;
				}
				break;
			case Engine3D::ShaderDataType::Int:
			case Engine3D::ShaderDataType::Int2:
			case Engine3D::ShaderDataType::Int3:
			case Engine3D::ShaderDataType::Int4:
			case Engine3D::ShaderDataType::Bool:
				{
					glEnableVertexAttribArray(index);
					glVertexAttribIPointer(index,
						element.GetComponentCount(),
						shaderDatatTypeToOpenGlBaseTypeConversion(element.type),
						layout.getStride(),
						(const void*)element.offset);
					index++;
				}
				break;
			default:
				/* assert(false); */
				break;
			}

        }

        _vertexBuffers.push_back(vertexBuffer);
    }

    void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) {
        glBindVertexArray(_rendererID);
        indexBuffer->Bind();

        _indexBuffer = indexBuffer;
    }

};
