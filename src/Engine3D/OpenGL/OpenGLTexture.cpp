#include <Engine3D/Engine3DPrecompiledHeader.h>
#include <OpenGL/OpenGLTexture.h>

namespace Engine3D{
	OpenGLTexture2D::OpenGLTexture2D(uint32_t w, uint32_t h) : image(w, h){
		ENGINE_PROFILE_FUNCTION();
        image.SetVerticalOnLoad(true);
        width = image.GetWidth();
        height = image.GetHeight();
		
        // Uploading data to OpenGL texture
		// internalData is our internalFormat
        internalFormat = GL_RGBA8;
		formatData = GL_RGBA;

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		// Since we segfault using glTextureParameteri, we'll use this for now
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	}

    OpenGLTexture2D::OpenGLTexture2D(const std::string& filepath) : image(filepath) {
		ENGINE_PROFILE_FUNCTION();

        if(!image.IsLoaded()){
            coreLogWarn("Could not load image to be used for creating a texture!");
        }

        image.SetVerticalOnLoad(true);

        int channels = image.GetPixelChannels();
        width = image.GetWidth();
        height = image.GetHeight();

        if(channels == 4){
            // internalData = GL_RGBA8;
            // formatData = GL_RGBA;
            internalFormat = GL_RGBA8;
            formatData = GL_RGBA;
        }
        else if(channels == 3){
            // internalData = GL_RGB8;
            // formatData = GL_RGB;
            internalFormat = GL_RGB8;
            formatData = GL_RGB;
        }

        glGenTextures(1, &id); // Equivalent to glCreateTexture (but will segfault though)
        glActiveTexture(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, id);

        //! @note Works on OpenGL 4.1 or higher
		// /* glCreateTextures(GL_TEXTURE_2D, 1, &id); */
		// /* glTextureStorage2D(GL_TEXTURE_2D, 0, _internalFormat, _width, _height); */
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		// Since we segfault using glTextureParameteri, we'll use this for now
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, formatData, GL_UNSIGNED_BYTE, image.GetImageData()); // same thing as doing: glTextureSubImage2D

        glGenerateMipmap(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    OpenGLTexture2D::~OpenGLTexture2D(){
		ENGINE_PROFILE_FUNCTION();

        glDeleteTextures(1, &id);
    }

    uint32_t OpenGLTexture2D::GetRendererIDInternal() const { return id; }
    uint32_t OpenGLTexture2D::GetWidthInternal() const { return width; }
    uint32_t OpenGLTexture2D::GetHeightInternal() const { return height; }

	void OpenGLTexture2D::SetDataInternal(void* data, uint32_t size){
		ENGINE_PROFILE_FUNCTION();

		/* glTextureSubImage2D(id, 0, 0, 0, _width, _height, */
		
		// bytes per pixel
		uint32_t bpp = formatData == GL_RGBA ? 4 : 3;

        assert((size == width * height * bpp));
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, formatData, GL_UNSIGNED_BYTE, data); // same thing as doing: glTextureSubImage2D
	}

    // Binding specific slot of this texture
    void OpenGLTexture2D::bind(GLenum slot) const {
		ENGINE_PROFILE_FUNCTION();

        /* glBindTextureUnit(slot, _rendererID); // Equivalent to glBindTexture (but will segfault though) */

		// Since we are using OpenGL 4.5 or lower, and cannot use glBindTextureUnit, we have to do the following two lines below
		// Essentially how textures are working with the batch renderer system, now.
		glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, id); // Instead of using this we have to use glBindTexture on Mac
    }

    bool OpenGLTexture2D::IsTextureLoadedSuccessful() const { return image.IsLoaded(); }

};
