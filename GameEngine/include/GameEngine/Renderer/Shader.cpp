#include <GameEngine/Renderer/Shader.h>
#include <glad/glad.h>

namespace RendererEngine{
    // Read our shaders into the appropriate buffers

    // In this constructor is how we create a shader.
    // 1.) Using OpenGL's function called glCreateShader() to create an empty shader
    //      - Doesn';t just create an ID, but does create unique ID's, etc
    // 2.) generateBuffer, then generateBind is the typical order
    // 3.) When creating a shader, we then attach it.
    // 4.) Then once we are finished with that shader, we then dettach it

    Shader::Shader(const std::string& vertexSrc, const std::string& fragmentSrc){
        coreLogInfo("Shader Initialized!");
        // Create an empty vertex shader handle
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

        // Send the vertex shader source code to GL
        // Note that std::string's .c_str is NULL character terminated.
        const GLchar *source = vertexSrc.c_str(); // ID for vertex
        glShaderSource(vertexShader, 1, &source, 0);

        // Compile the vertex shader
        glCompileShader(vertexShader);

        // We check compilation status if the shader compiled successfully
        GLint isCompiled = 0;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);

        // If shader didnt compile we know it doesnt compile because we have the infolog
        // - this log gives us the size and infoLog to give us the error message of how come the shader didn't work
        //   and was unsuccessful./
        if(isCompiled == GL_FALSE){
            GLint maxLength = 0;
            glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);
            
            // We don't need the shader anymore.
            glDeleteShader(vertexShader);

            coreLogError("Vertex Shader compilation failure! (In Shader.cpp)");
            coreLogError("{}", infoLog.data());
            render_core_assert(false, "Vertex shader compilation error!");
            return;
        }

        // Create an empty fragment shader handle
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

        // Send the fragment shader source code to GL
        // Note that std::string's .c_str is NULL character terminated.
        source = fragmentSrc.c_str();
        glShaderSource(fragmentShader, 1, &source, 0);

        // Compile the fragment shader
        glCompileShader(fragmentShader);

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);

        if (isCompiled == GL_FALSE){
            GLint maxLength = 0;
            glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);
            
            // We don't need the shader anymore.
            glDeleteShader(fragmentShader);
            // Either of them. Don't leak shaders.
            glDeleteShader(vertexShader);

            coreLogError("Fragment Shader compilation failure!");
            coreLogError("{}", infoLog.data());
            render_core_assert(false, "Fragment shader compilation error!");
            return;
        }

        // Vertex and fragment shaders are successfully compiled.
        // Now time to link them together into a program.
        // Get a program object.
        // NOTE to self: we want to store the renderer ID with the shader program "glCreateProgram()"
        _rendererID= glCreateProgram();
        GLint program = _rendererID;

        // Attach our shaders to our program
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);

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
            // Don't leak shaders either.
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);

            coreLogError("Fragment Shader link failure!");
            coreLogError("{}", infoLog.data());
            render_core_assert(false, "Shader link error!");
            return;
        }

        // Always detach shaders after a successful link.
        glDetachShader(program, vertexShader);
        glDetachShader(program, fragmentShader);
    }

    Shader::~Shader(){
        glDeleteProgram(_rendererID);
    }

    void Shader::bind() const {
        glUseProgram(_rendererID);
    }

    void Shader::unbind() const {
        glUseProgram(0);
    }
};