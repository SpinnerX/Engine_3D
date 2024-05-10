#pragma once
#include <Engine3D/Graphics/Shader.h>
#include <GLFW/glfw3.h>

namespace Engine3D{
    // Shader class (Specific to OpenGL)
    // Eventually we are creating a renderer where other Renderer API's can customize and use these functions
    // - basically be implemented per API
    class OpenGLShader : public Shader{
    public:
        OpenGLShader(const std::string& filepath);
        OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
        ~OpenGLShader();

    private:
        void bind() const override;
        void unbind() const override;

        std::string GetNameInternal() const override;

        void UploadUniformInt(const std::string& name, int values) override;
		void UploadIntArray(const std::string& name, int* values, uint32_t count) override;
        void UploadUniformFloat(const std::string& name, float values) override;
        void UploadUniformFloat2(const std::string& name, const glm::vec2& values) override;
        void UploadUniformFloat3(const std::string& name, const glm::vec3& values) override;
        void UploadUniformFloat4(const std::string& name, const glm::vec4& values) override;

        void UploadUniformMat3(const std::string& name, const glm::mat3& matrix) override;
        void UploadUniformMat4(const std::string& name, const glm::mat4& matrix) override;

    private:
        std::string readFile(const std::string& filepath);
        std::unordered_map<GLenum, std::string> preProcess(const std::string& src);
        /* void compile(const std::unordered_map<GLenum, std::string>& map); */
		void compileOrGetVulkanBinaries(const std::unordered_map<GLenum, std::string>& shaderSrcs);

		void compileOrGetOpenGLBinaries();
		
		void createProgram();

		void reflect(GLenum stage, const std::vector<uint32_t>& shaderData);
    private:
        uint32_t id; // Keeping track uniquely identifying this object
        std::string name;
		std::string filepath; // Keeping track of the filepath.
    };
};
