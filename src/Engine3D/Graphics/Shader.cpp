#include <Engine3D/Engine3DPrecompiledHeader.h>
#include <Engine3D/Graphics/Shader.h>

namespace Engine3D{

    Ref<Shader> Shader::Create(const std::string& filepath){
        switch (RendererAPI::getAPI()){
            case RendererAPI::API::OpenGL: return CreateRef<OpenGLShader>(filepath);
        }

        // render_core_assert(false, "Unknown renderer API");
        assert(false);
        return nullptr;
    }
    
    Ref<Shader> Shader::CreateShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc){
        switch (RendererAPI::getAPI()){
            case RendererAPI::API::OpenGL:
                return CreateRef<OpenGLShader>(name, vertexSrc, fragmentSrc);
        }

        // render_core_assert(false, "Unknown renderer API");
        assert(false);
        return nullptr;
    }

    void Shader::Bind() const{
        bind();
    }

    void Shader::Unbind() const {
        unbind();
    }

    std::string Shader::GetName() const {
        return GetNameInternal();
    }

    void Shader::Set(const std::string& name, int value){
        UploadUniformInt(name, value);
    }

    void Shader::Set(const std::string& name, int* values, uint32_t count){
        UploadIntArray(name, values, count);
    }

    void Shader::Set(const std::string& name, float value){
        UploadUniformFloat(name, value);
    }

    void Shader::Set(const std::string& name, const glm::vec2& value){
        UploadUniformFloat2(name, value);
    }
		
    void Shader::Set(const std::string& name, const glm::vec3& value){
        UploadUniformFloat3(name, value);
    }

    void Shader::Set(const std::string& name, const glm::vec4& value){
        UploadUniformFloat4(name, value);
    }

    void Shader::Set(const std::string& name, const glm::mat3& value){
        UploadUniformMat3(name, value);
    }

    void Shader::Set(const std::string& name, const glm::mat4& value){
        UploadUniformMat4(name, value);
    }

    //////////////////////
    /// Shader Library ///
    //////////////////////

    void ShaderLibrary::Add(const Ref<Shader>& shader){
        auto name = shader->GetName();
        if(contains(name)) coreLogError("Shader already exists");
        shaders[name] = shader;
    }

    void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader){
        if(contains(name)) coreLogError("Shader aready exists");

        shaders[name] = shader;
    }

    Ref<Shader> ShaderLibrary::Load(const std::string& filepath){
        auto shader = Shader::Create(filepath);
        Add(shader);
        return shader;
    }

    void ShaderLibrary::Load(const std::string& name, const std::string& filepath){
        auto shader = Shader::Create(filepath);
        Add(name, shader);
    }

    Ref<Shader>& ShaderLibrary::Get(const std::string& name){
        if(!contains(name)){
            coreLogError("Shader not found");
        }

        return shaders[name];
    }

    bool ShaderLibrary::contains(const std::string& name){
        return shaders.contains(name);
    }
};
