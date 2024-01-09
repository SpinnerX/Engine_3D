#include <GameEnginePrecompiledHeader.h>
#include <GameEngine/platforms/OpenGL/OpenGLShader.h>
#include <fstream>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace RendererEngine{
    // Read our shaders into the appropriate buffers

    // In this constructor is how we create a shader.
    // 1.) Using OpenGL's function called glCreateShader() to create an empty shader
    //      - Doesn';t just create an ID, but does create unique ID's, etc
    // 2.) generateBuffer, then generateBind is the typical order
    // 3.) When creating a shader, we then attach it.
    // 4.) Then once we are finished with that shader, we then dettach it

    static GLenum shaderTypeFromString(const std::string& type){
        if(type == "vertex")
            return GL_VERTEX_SHADER;
        if(type == "fragment" || type == "pixel")
            return GL_FRAGMENT_SHADER;
        
        render_core_assert(false, "Unknown shader type!");
        return 0;
    }

    OpenGLShader::OpenGLShader(const std::string& filepath){
        std::string src = readFile(filepath);
        std::unordered_map<GLenum, std::string> shaderSrc = preProcess(src);
        compile(shaderSrc);

        // Examples Filepath: asets/shaders/Texture.glsl
        // Essentially how we will extract Texture.glsl from the filepath.
        auto lastSlash = filepath.find_last_of("/\\");
        lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;

        auto lastDot = filepath.rfind('.');
        // If no dot found, then the count is the filepath.size() - lastSlash
        // Though if there is a dot (.), then we go backwards until we hit our back slash and that'll be our count
        auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;

        // This is how we get our name, when every time we load in our shader
        // The name of the file is our key, and the actual shader, is our value in unordered map
        // In Short: Name will be used to get the specific shader, that we're storing
        _name = filepath.substr(lastSlash, count);

        coreLogTrace("Filepath: {}", _name);
    }

    OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) : _name(name){
		RENDER_PROFILE_FUNCTION();

        std::unordered_map<GLenum, std::string> sources;
        sources[GL_VERTEX_SHADER] = vertexSrc;
        sources[GL_FRAGMENT_SHADER] = fragmentSrc;

        compile(sources);

    }

    OpenGLShader::~OpenGLShader(){
		RENDER_PROFILE_FUNCTION();
        glDeleteProgram(_rendererID);
    }

    std::string OpenGLShader::readFile(const std::string& filepath){
		RENDER_PROFILE_FUNCTION();
        std::ifstream ins(filepath, std::ios::in | std::ios::binary);
        std::string result = "";

        if(!ins){
            coreLogError("File was not able to be loaded in OpenGLShader(const string&)");
            coreLogError("Could not open filepath: {}\n", filepath);
        }
        else{
            ins.seekg(0, std::ios::end);
            result.resize(ins.tellg());
            ins.seekg(0, std::ios::beg);
            ins.read(&result[0], result.size());
            ins.close();
        }

        return result;
    }

    std::unordered_map<GLenum, std::string> OpenGLShader::preProcess(const std::string& src){
		RENDER_PROFILE_FUNCTION();
        std::unordered_map<GLenum, std::string> shaderSources;
        const char* typeToken = "#type";
        size_t typeTokenLength = strlen(typeToken);
        size_t pos = src.find(typeToken, 0);

        while(pos != std::string::npos){
            size_t eol = src.find_first_of("\r\n", pos);
            render_core_assert(eol != std::string::npos, "Syntax Error!");

            size_t begin = pos + typeTokenLength + 1; // being the beginning of the token
            std::string type = src.substr(begin, eol - begin); // extracting that actual string type
            render_core_assert(shaderTypeFromString(type), "Invalid shader type specifier");

            size_t nextLinePos = src.find_first_not_of("\r\n", eol);
            pos = src.find(typeToken, nextLinePos);
            shaderSources[shaderTypeFromString(type)] = src.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? src.size()-1 : nextLinePos));
        }

        return shaderSources;
    }

    void OpenGLShader::compile(const std::unordered_map<GLenum, std::string>& shaderSources){
		RENDER_PROFILE_FUNCTION();
        GLint program = glCreateProgram();
        render_core_assert(shaderSources.size() <= 2, "We only support 2 shaders for now!");

        std::array<GLenum, 3> glShaderIDs;
        int glShaderIDIndex = 0; // Keeping track of our shaders

        // Iterating the shader sources to create our shaders
        for(auto& [id, value] : shaderSources){
            GLenum type = id;
            const std::string& src = value;
            GLuint shader = glCreateShader(type);

            const GLchar *sourceCStr = src.c_str(); // ID for vertex
            glShaderSource(shader, 1, &sourceCStr, 0);

            glCompileShader(shader);

            GLint isCompiled = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);

            // Checking if compiling our shaders ended up failing
            // Then we log that information if it failed.
            if(isCompiled == GL_FALSE){
                GLint maxLength = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

                std::vector<GLchar> infoLog(maxLength);
                glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);
                
                glDeleteShader(shader);

                coreLogError("Vertex Shader compilation failure! (In Shader.cpp)");
                coreLogError("{}", infoLog.data());
                render_core_assert(false, "Shader compilation error!");
                break;
            }

            glAttachShader(program, shader);
            glShaderIDs[glShaderIDIndex++] = shader;
        }

        // Link our program
        glLinkProgram(program);

        // Note the different functions here: glGetProgram* instead of glGetShader*.
        GLint isLinked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, (int *)&isLinked);

        if (isLinked == GL_FALSE){
            GLint maxLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
            
            // We don't need the program anymore.
            glDeleteProgram(program);
            
            for(GLenum id : glShaderIDs){
                glDeleteShader(id);
            }

            coreLogError("Fragment Shader link failure!");
            coreLogError("{}", infoLog.data());
            render_core_assert(false, "Shader link error!");
            return;
        }

        for(GLenum id : glShaderIDs){
            glDetachShader(program, id);
        }
        _rendererID = program;
    }

    void OpenGLShader::bind() const {
		RENDER_PROFILE_FUNCTION();
        glUseProgram(_rendererID);
    }

    void OpenGLShader::unbind() const {
        glUseProgram(0);
    }


	void OpenGLShader::setInt(const std::string& name, int value){
		RENDER_PROFILE_FUNCTION();
		uploadUniformInt(name, value);
	}
		
	void OpenGLShader::setFloat(const std::string& name, float value) {
		uploadUniformFloat(name, value);
	}
	
	void OpenGLShader::setFloat3(const std::string& name, const glm::vec3& value){
		RENDER_PROFILE_FUNCTION();
		uploadUniformFloat3(name, value);
	}

	void OpenGLShader::setFloat4(const std::string& name, const glm::vec4& value){
		RENDER_PROFILE_FUNCTION();
		uploadUniformFloat4(name, value);
	}

	void OpenGLShader::setMat4(const std::string& name, const glm::mat4& value){
		RENDER_PROFILE_FUNCTION();
		uploadUniformMat4(name, value);
	}

    void OpenGLShader::uploadUniformInt(const std::string& name, int value){
        GLint location = glGetUniformLocation(_rendererID, name.c_str());
        glUniform1i(location, value);
    }

    void OpenGLShader::uploadUniformFloat(const std::string& name, float value){
        GLint location = glGetUniformLocation(_rendererID, name.c_str());
        glUniform1f(location, value);
    }

    void OpenGLShader::uploadUniformFloat2(const std::string& name, const glm::vec2& values){
        GLint location = glGetUniformLocation(_rendererID, name.c_str());
        glUniform2f(location, values.x, values.y);
    }

    void OpenGLShader::uploadUniformFloat3(const std::string& name, const glm::vec3& values){
        GLint location = glGetUniformLocation(_rendererID, name.c_str());
        glUniform3f(location, values.x, values.y, values.z);
    }

    void OpenGLShader::uploadUniformFloat4(const std::string& name, const glm::vec4& values){
        GLint location = glGetUniformLocation(_rendererID, name.c_str());
        glUniform4f(location, values.x, values.y, values.z, values.w);
    }

    void OpenGLShader::uploadUniformMat3(const std::string& name, const glm::mat3& matrix){
        GLint location = glGetUniformLocation(this->_rendererID, name.c_str()); // validates if this exists
        glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void OpenGLShader::uploadUniformMat4(const std::string& name, const glm::mat4& matrix){
        GLint location = glGetUniformLocation(this->_rendererID, name.c_str()); // validates if this exists
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }
};
