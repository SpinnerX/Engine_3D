#include <Engine3D/Engine3DPrecompiledHeader.h>
#include <OpenGL/OpenGLShader.h>

namespace Engine3D{
    // Read our shaders into the appropriate buffers

    // In this constructor is how we create a shader.
    // 1.) Using OpenGL's function called glCreateShader() to create an empty shader
    //      - Doesn';t just create an ID, but does create unique ID's, etc
    // 2.) generateBuffer, then generateBind is the typical order
    // 3.) When creating a shader, we then attach it.
    // 4.) Then once we are finished with that shader, we then dettach it

    static GLenum shaderTypeFromString(const std::string& type){
        if(type == "vertex") return GL_VERTEX_SHADER;
        if(type == "fragment" || type == "pixel") return GL_FRAGMENT_SHADER;

        coreLogWarn("Unknown Shader Type!");
        assert(false);
        return 0;
    }

    static std::string shaderTypeToString(const GLenum type){
        switch (type){
        case GL_VERTEX_SHADER: return "GL_VERTEX_SHADER";
        case GL_FRAGMENT_SHADER: return "GL_FRAGMENT_SHADER";
        }
        coreLogWarn("Unknown Shader Type!");
        assert(false);
        return "";
    }

	// static shaderc_shader_kind glShaderStageToShaderC(GLenum stage){
	// 	switch (stage) {
	// 		case GL_VERTEX_SHADER: return shaderc_glsl_vertex_shader;
	// 		case GL_FRAGMENT_SHADER: return shaderc_glsl_fragment_shader;
	// 	}
	// 	return (shaderc_shader_kind)0;
	// }

	static const char* glShaderStageToString(GLenum stage){
		switch (stage) {
			case GL_VERTEX_SHADER: return "GL_VERTEX_SHADER";
			case GL_FRAGMENT_SHADER: return "GL_FRAGMENT_SHADER";
		}
		assert(false);
		return nullptr;
	}

	static const char* getCacheDirectory(){
		// TODO: make sure assets directory is valid.
		return "assets/cache/shader/opengl";
	}

    OpenGLShader::OpenGLShader(const std::string& filepath) : filepath(filepath){
        std::string src = readFile(filepath);
        auto shaderSrc = preProcess(src);
		RENDER_PROFILE_FUNCTION();

		{

			Timer timer;
			compileOrGetVulkanBinaries(shaderSrc);
			compileOrGetOpenGLBinaries();
			createProgram();

			coreLogWarn("Shader Creation Took {0} ms", timer.elapsedMilliseconds());
		}


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
        name = filepath.substr(lastSlash, count);

        coreLogTrace("Filepath: {}", name);
    }

    OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) : name(name){
		RENDER_PROFILE_FUNCTION();

        std::unordered_map<GLenum, std::string> sources;
        sources[GL_VERTEX_SHADER] = vertexSrc;
        sources[GL_FRAGMENT_SHADER] = fragmentSrc;

        compileOrGetVulkanBinaries(sources);
		compileOrGetOpenGLBinaries();
		createProgram();

    }

    OpenGLShader::~OpenGLShader(){
		RENDER_PROFILE_FUNCTION();
        glDeleteProgram(id);
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

        //! @note We start parsing at the beginning of the file looking for #type
        //! @note This "#type" allows us to identify if the shader we are parsing are a vertex or fragment shader
        size_t pos = src.find(typeToken, 0);

        //! @note Then we parse from our starting position
        while(pos != std::string::npos){
            size_t eol = src.find_first_of("\r\n", pos);
            // render_core_assert(eol != std::string::npos, "Syntax Error!");
            assert(eol != std::string::npos);

            size_t begin = pos + typeTokenLength + 1; // being the beginning of the token
            std::string type = src.substr(begin, eol - begin); // extracting that actual string type
            // render_core_assert(shaderTypeFromString(type), "Invalid shader type specifier");
            assert(shaderTypeFromString(type));

            size_t nextLinePos = src.find_first_not_of("\r\n", eol);
            pos = src.find(typeToken, nextLinePos);
            shaderSources[shaderTypeFromString(type)] = src.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? src.size()-1 : nextLinePos));
            coreLogWarn("Shader Type To String is {}", shaderTypeToString(shaderTypeFromString(type)));
        }

        return shaderSources;
    }

    void OpenGLShader::compileOrGetVulkanBinaries(const std::unordered_map<GLenum, std::string>& sources){
		RENDER_PROFILE_FUNCTION();
        GLint program = glCreateProgram();

		std::array<GLenum, 3> shaderIDs;
		int glShaderIDIndex = 0; //  Keeping track of shaders

		for(auto& [t, value] : sources){
			GLenum type = t;
			const std::string& src = value;
			GLuint shader = glCreateShader(type);

			const GLchar* sourceCStr = src.c_str();

			glShaderSource(shader, 1, &sourceCStr, 0);
			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);

			if(isCompiled == GL_FALSE){
                GLint maxLength = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

                std::vector<GLchar> infoLog(maxLength);
                glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);
                
                glDeleteShader(shader);

                coreLogError("Vertex Shader compilation failure! (In Shader.cpp)");
                coreLogError("{}", infoLog.data());
				// assert(false);
                break;
            }

			glAttachShader(program, shader);
            shaderIDs[glShaderIDIndex++] = shader;
		}

		glLinkProgram(program);

		GLint isLinked =0;
		glGetProgramiv(program, GL_LINK_STATUS, (int *)&isLinked);

		if(isLinked == GL_FALSE){
			GLint maxLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
            
            // We don't need the program anymore.
            glDeleteProgram(program);
            
            for(GLenum shaderID : shaderIDs){
                glDeleteShader(shaderID);
            }

            coreLogError("Shader link failure!");
            coreLogError("{}", infoLog.data());
			// assert(false);
            return;
		}

		for(GLenum shaderID : shaderIDs){
			glDetachShader(program, shaderID);
		}

		id = program;

    }
	
	void OpenGLShader::compileOrGetOpenGLBinaries(){}
		
	void OpenGLShader::createProgram(){}

	void OpenGLShader::reflect(GLenum stage, const std::vector<uint32_t>& shaderData){}

    void OpenGLShader::bind() const {
		RENDER_PROFILE_FUNCTION();
        glUseProgram(id);
    }

    void OpenGLShader::unbind() const {
        glUseProgram(0);
    }

    std::string OpenGLShader::GetNameInternal() const { return name; }

    void OpenGLShader::UploadUniformInt(const std::string& name, int value){
        GLint location = glGetUniformLocation(id, name.c_str());
        glUniform1i(location, value);
    }
	
	void OpenGLShader::UploadIntArray(const std::string& name, int* values, uint32_t count){
        GLint location = glGetUniformLocation(id, name.c_str());
        glUniform1iv(location, count, values);
		
	}

    void OpenGLShader::UploadUniformFloat(const std::string& name, float value){
        GLint location = glGetUniformLocation(id, name.c_str());
        glUniform1f(location, value);
    }

    void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& values){
        GLint location = glGetUniformLocation(id, name.c_str());
        glUniform2f(location, values.x, values.y);
    }

    void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& values){
        GLint location = glGetUniformLocation(id, name.c_str());
        glUniform3f(location, values.x, values.y, values.z);
    }

    void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& values){
        GLint location = glGetUniformLocation(id, name.c_str());
        glUniform4f(location, values.x, values.y, values.z, values.w);
    }

    void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix){
        GLint location = glGetUniformLocation(this->id, name.c_str()); // validates if this exists
        glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix){
        GLint location = glGetUniformLocation(this->id, name.c_str()); // validates if this exists
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }
};
