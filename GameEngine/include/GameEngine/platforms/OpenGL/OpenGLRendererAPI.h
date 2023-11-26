#pragma once
#include <GameEngine/Renderer/RendererAPI.h>

namespace RendererEngine{

    class OpenGLRendererAPI : public RendererAPI{
    public:

        virtual void setClearColor(const glm::vec4& color) override;
        virtual void clear() override;

        virtual void drawIndexed(const std::shared_ptr<VertexArray>& vertexArr) override;
    };

};