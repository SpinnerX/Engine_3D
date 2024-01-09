#pragma once
#include <string>
#include <glm/glm.hpp>
#include <unordered_map>

namespace RendererEngine{

    // Creating a Shader class
    // Eventually we are creating a renderer where other Renderer API's can customize and use these functions
    // - basically be implemented per API
    // - contains a name because so that an asset can have an id specifier
    class Shader{
    public:
        virtual ~Shader() = default;

        virtual void bind() const = 0;

        virtual void unbind() const = 0;
		
		virtual void setInt(const std::string& name, int value) = 0;
		
		virtual void setFloat(const std::string& name, float) = 0;
		
		virtual void setFloat3(const std::string& name, const glm::vec3& value) = 0;

		virtual void setFloat4(const std::string& name, const glm::vec4& value) = 0;

		virtual void setMat4(const std::string& name, const glm::mat4& value) = 0;

        virtual const std::string& getName() const = 0;

        static Ref<Shader> CreateShader(const std::string& path);
        static Ref<Shader> CreateShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);

    private:
        uint32_t _rendererID; // Keeping track uniquely identifying this object
    };

    // Shader Library
    // - Handling and automating shaders
    // - Make easier to create, load, and modify shaders
    class ShaderLibrary{
    public:

        void add(const Ref<Shader>& shader);
        void add(const std::string& name, const Ref<Shader>& shader);
        Ref<Shader> load(const std::string& filepath);
        void load(const std::string& name, const std::string& filepath);

        Ref<Shader>& get(const std::string& name);

        // Just to
        bool exists(const std::string& name);

    private:
        std::unordered_map<std::string, Ref<Shader>> _shaders;
    };
};
