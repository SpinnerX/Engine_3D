#pragma once
#include <string>

namespace RendererEngine{

    // Creating a Shader class
    // Eventually we are creating a renderer where other Renderer API's can customize and use these functions
    // - basically be implemented per API
    class Shader{
    public:
        // In the future we should take these in as a file.
        Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
        ~Shader();

        void bind() const;

        void unbind() const;
    private:
        uint32_t _rendererID; // Keeping track uniquely identifying this object
    };
};