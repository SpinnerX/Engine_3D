#include <GameEnginePrecompiledHeader.h>
#include <GameEngine/platforms/OpenGL/OpenGLTexture.h>
#include <GameEngine/image/stb_image.h>

namespace RendererEngine{
	OpenGLTexture2D::OpenGLTexture2D(uint32_t w, uint32_t h) : _width(w), _height(h) {
		RENDER_PROFILE_FUNCTION();
		
        // Uploading data to OpenGL texture
		// internalData is our internalFormat
        _internalFormat = GL_RGBA8;
		_formatData = GL_RGBA;

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		
		// Since we segfault using glTextureParameteri, we'll use this for now
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	}

    OpenGLTexture2D::OpenGLTexture2D(const std::string& filepath) : _filepath(filepath) {
		RENDER_PROFILE_FUNCTION();

        coreLogInfo("OpenGLTexture2D Create Called!");
        int width, height, channels;

        stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = nullptr;
		{
		RENDER_PROFILE_SCOPE("stbi_load - OpenGLTexture2D::OpenGLTexture2D(const std::string&)");
        // Loading in our image data
        data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
		}

        if(data == nullptr){
            coreLogError("Failed to load image as the value was nullptr!");
        }
        else coreLogInfo("Image Data was not nullptr meaning it was valid!");
        render_core_assert(data, "Failed to load image!");

        _width = width;
        _height = height;

        // Uploading data to OpenGL texture
		// internalData is our internalFormat
        GLenum internalData = 0, formatData = 0;

        if(channels == 4){
            internalData = GL_RGBA8;
            formatData = GL_RGBA;
        }
        else if(channels == 3){
            internalData = GL_RGB8;
            formatData = GL_RGB;
        }

		_internalFormat = internalData, _formatData = formatData;

        // Testing textures for OpenGL
        glGenTextures(1, &_rendererID); // Equivalent to glCreateTexture (but will segfault though)
        glActiveTexture(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, _rendererID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		
		// Since we segfault using glTextureParameteri, we'll use this for now
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		/* glTextureParameteri(_rendererID, GL_TEXTURE_WRAP_S, GL_REPEAT); */
		/* glTextureParameteri(_rendererID, GL_TEXTURE_WRAP_T, GL_REPEAT); */

        glTexImage2D(GL_TEXTURE_2D, 0, internalData, _width, _height, 0, formatData, GL_UNSIGNED_BYTE, data); // same thing as doing: glTextureSubImage2D

        glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data); // free image data stored in CPU

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    OpenGLTexture2D::~OpenGLTexture2D(){
		RENDER_PROFILE_FUNCTION();

        glDeleteTextures(1, &_rendererID);
    }



	void OpenGLTexture2D::setData(void* data, uint32_t size){
		RENDER_PROFILE_FUNCTION();

		/* glTextureSubImage2D(_rendererID, 0, 0, 0, _width, _height, */ 
		
		// bytes per pixel
		uint32_t bpp = _formatData == GL_RGBA ? 4 : 3;
		render_core_assert(size == _width * _height * bpp, "Data must be an entire texture"); // Make sure that the textures are fine for rn
        glTexImage2D(GL_TEXTURE_2D, 0, _internalFormat, _width, _height, 0, _formatData, GL_UNSIGNED_BYTE, data); // same thing as doing: glTextureSubImage2D
	}

    // Binding specific slot of this texture
    void OpenGLTexture2D::bind(GLenum slot) const {
		RENDER_PROFILE_FUNCTION();

        // printf("Slot: %i\n", slot);
        // glBindTextureUnit(slot, _rendererID); // Equivalent to glBindTexture (but will segfault though)
        glBindTexture(slot, _rendererID); // Instead of using this we have to use glBindTexture on Mac
    }
};
