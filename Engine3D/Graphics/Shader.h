#pragma once
#include <Engine3D/Core/core.h>
#include <Engine3D/Core/EngineLogger.h>
#include <string>
#include <glm/glm.hpp>
#include <unordered_map>

namespace Engine3D{

    // Creating a Shader class
    // Eventually we are creating a renderer where other Renderer API's can customize and use these functions
    // - basically be implemented per API
    // - contains a name because so that an asset can have an id specifier
    class Shader{
    public:
        virtual ~Shader() = default;

        void Bind() const;

        void Unbind() const;
		
		void Set(const std::string& name, int value);
		
		void Set(const std::string& name, int* values, uint32_t count);

		void Set(const std::string& name, float);

        void Set(const std::string& name, const glm::vec2& value);
		
		void Set(const std::string& name, const glm::vec3& value);

		void Set(const std::string& name, const glm::vec4& value);

        void Set(const std::string& name, const glm::mat3& value);
		
        void Set(const std::string& name, const glm::mat4& value);

        std::string GetName() const;

        static Ref<Shader> Create(const std::string& path);
        static Ref<Shader> CreateShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);

    private:

        virtual void bind() const = 0;
        virtual void unbind() const = 0;

        virtual std::string GetNameInternal() const = 0;

        virtual void UploadUniformInt(const std::string& name, int values) = 0;
		virtual void UploadIntArray(const std::string& name, int* values, uint32_t count) = 0;
        virtual void UploadUniformFloat(const std::string& name, float values) = 0;
        virtual void UploadUniformFloat2(const std::string& name, const glm::vec2& values) = 0;
        virtual void UploadUniformFloat3(const std::string& name, const glm::vec3& values) = 0;
        virtual void UploadUniformFloat4(const std::string& name, const glm::vec4& values) = 0;

        virtual void UploadUniformMat3(const std::string& name, const glm::mat3& matrix) = 0;
        virtual void UploadUniformMat4(const std::string& name, const glm::mat4& matrix) = 0;
    };

    // Shader Library
    // - Handling and automating shaders
    // - Make easier to create, load, and modify shaders
    class ShaderLibrary{
    public:

        void Add(const Ref<Shader>& shader);
        void Add(const std::string& name, const Ref<Shader>& shader);
        Ref<Shader> Load(const std::string& filepath);
        void Load(const std::string& name, const std::string& filepath);

        Ref<Shader>& Get(const std::string& name);

        // Just to
        bool contains(const std::string& name);

    private:
        std::unordered_map<std::string, Ref<Shader>> shaders;
    };
};
