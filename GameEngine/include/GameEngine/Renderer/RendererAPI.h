#pragma once
#include <memory>
#include <glm/glm.hpp>
#include <GameEngine/Renderer/VertexArray.h>

namespace RendererEngine{

    // RendererAPI
    // - is a pure virtual class that should have no implementation
    // - Going to have an implementation per platform (such as one for opengl, and one for DirectX)
    // - Series of command of renderer calls
    // - clear() is whhat will clear the the buffer
    class RendererAPI{
    public:
        // Tell what API we are using
        enum class API{
            None = 0, // if you're running headless, or you just don't want to be running rendering because unit tests dont use rendering
            OpenGL = 1 //if we only want to run OpenGL rendering API's
        };
    public:
        virtual void setClearColor(const glm::vec4& color) = 0;
        virtual void clear() = 0;

        virtual void drawIndexed(const std::shared_ptr<VertexArray>& vertexArr) = 0;

        inline static API getAPI() { return _api; }

    private:
        static API _api;
    };
};