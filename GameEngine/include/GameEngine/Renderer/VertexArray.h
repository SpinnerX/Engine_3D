#pragma once
#include <memory>
#include <GameEngine/Renderer/Buffer.h>

namespace RendererEngine{
    // VertexArray
    // - Essentially is a state entity interface that contains data
    // ** Some information what this interface will contain **
    // - Contains information like vertex, index buffers and references to these kind of data in the vertex array
    // - Buffers, and stores the layours corresponding with those vertex/index buffers in this vertex array
    // How Resources managements gonna work for this game engine, for right now...
    // - Is everything, resource related thing like vertex, index buffer, texture, shaders, and things like that
    //   are going to be referenced counted (shared_ptr)
    // ** NOTE ** - if you wanted to create a much more optimized approach then potentially creating some kind of 
    //              intruisive reference counting system.
    // ** for future reference in case there is another way to do this **
    // - Potentially changing the actual type name to be something else.
    class VertexArray{
    public:
        virtual ~VertexArray(){}

        virtual void bind() const = 0;
        virtual void unbind() const = 0; // NOTE: In vertex arrays there are use cases where you may want to unbind your data.

        virtual void addVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) = 0;
        virtual void setIndexBuffer(const std::shared_ptr<IndexBuffer>& vertexBuffer) = 0;

        virtual const std::vector<std::shared_ptr<VertexBuffer>>& getVertexBuffer() const  = 0;
        virtual const std::shared_ptr<IndexBuffer>& getIndexBuffer() const  = 0;

        static VertexArray* Create();

    };
};