#include <GameEnginePrecompiledHeader.h>
#include <GameEngine/platforms/OpenGL/OpenGLVertexArray.h>
#include <glad/glad.h>

namespace RendererEngine{
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

        render_core_assert(false, "Unknown ShaderDataType!");
        return 0;
    }

    OpenGLVertexArray::OpenGLVertexArray(){
        // glCreateVertexArrays(1, &_rendererID);
        glGenVertexArrays(1, &_rendererID);
    }

    OpenGLVertexArray::~OpenGLVertexArray(){
        glDeleteVertexArrays(1, &_rendererID);
    }

    void OpenGLVertexArray::bind() const {
        glBindVertexArray(_rendererID);
    }

    void OpenGLVertexArray::unbind() const {
        glBindVertexArray(0);
    }

    void OpenGLVertexArray::addVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) {
        render_core_assert(vertexBuffer->getLayout().getElements().size(), "Vertex buffer has no layout!");
        
        glBindVertexArray(_rendererID);

        vertexBuffer->bind();

        uint32_t index = 0;
        const auto& layout = vertexBuffer->getLayout();
        for(const auto& element : layout){
                glEnableVertexAttribArray(index);
                glVertexAttribPointer(index,
                                        element.getComponentCount(),
                                        shaderDatatTypeToOpenGlBaseTypeConversion(element.type),
                                        element.isNormalized ? GL_TRUE : GL_FALSE,
                                        vertexBuffer->getLayout().getStride(),
                                        (const void*)element.offset); // provind a vertex a shader
                index++;
        }

        _vertexBuffers.push_back(vertexBuffer);
    }

    void OpenGLVertexArray::setIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) {
        glBindVertexArray(_rendererID);
        indexBuffer->bind();

        _indexBuffer = indexBuffer;
    }

};