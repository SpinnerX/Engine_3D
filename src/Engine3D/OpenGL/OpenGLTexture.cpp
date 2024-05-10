#include <Engine3D/Engine3DPrecompiledHeader.h>
#include <OpenGL/OpenGLTexture.h>

namespace Engine3D{
	OpenGLTexture2D::OpenGLTexture2D(uint32_t w, uint32_t h) : width(w), height(h) {
		RENDER_PROFILE_FUNCTION();
		
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

    OpenGLTexture2D::OpenGLTexture2D(const std::string& filepath) : filepath(filepath) {
		RENDER_PROFILE_FUNCTION();

        coreLogInfo("OpenGLTexture2D Create Called!");
        int imageWidth, imageHeight, channels;

        stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = nullptr;
		{
		RENDER_PROFILE_SCOPE("stbi_load - OpenGLTexture2D::OpenGLTexture2D(const std::string&)");
        // Loading in our image data
        data = stbi_load(filepath.c_str(), &imageWidth, &imageHeight, &channels, 0);
		}

        if(data == nullptr){
            coreLogError("Failed to load image as the value was nullptr!");
        }
        else coreLogInfo("Image Data was not nullptr meaning it was valid!");
        // render_core_assert(data, "Failed to load image!");

        width = imageWidth;
        height = imageHeight;

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

		internalFormat = internalData, this->formatData = formatData;

        // Testing textures for OpenGL
        glGenTextures(1, &id); // Equivalent to glCreateTexture (but will segfault though)
        glActiveTexture(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, id);
		// /* glCreateTextures(GL_TEXTURE_2D, 1, &id); */
		// /* glTextureStorage2D(GL_TEXTURE_2D, 0, _internalFormat, _width, _height); */
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		// Since we segfault using glTextureParameteri, we'll use this for now
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		/* glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_REPEAT); */
		/* glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_REPEAT); */

        glTexImage2D(GL_TEXTURE_2D, 0, internalData, width, height, 0, formatData, GL_UNSIGNED_BYTE, data); // same thing as doing: glTextureSubImage2D

        glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data); // free image data stored in CPU

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    OpenGLTexture2D::~OpenGLTexture2D(){
		RENDER_PROFILE_FUNCTION();

        glDeleteTextures(1, &id);
    }



	void OpenGLTexture2D::SetDataInternal(void* data, uint32_t size){
		RENDER_PROFILE_FUNCTION();

		/* glTextureSubImage2D(id, 0, 0, 0, _width, _height, */ 
		
		// bytes per pixel
		uint32_t bpp = formatData == GL_RGBA ? 4 : 3;
		// render_core_assert(size == _width * _height * bpp, "Data must be an entire texture"); // Make sure that the textures are fine for rn
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, formatData, GL_UNSIGNED_BYTE, data); // same thing as doing: glTextureSubImage2D
	}

    // Binding specific slot of this texture
    void OpenGLTexture2D::bind(GLenum slot) const {
		RENDER_PROFILE_FUNCTION();

        /* glBindTextureUnit(slot, _rendererID); // Equivalent to glBindTexture (but will segfault though) */

		// Since we are using OpenGL 4.5 or lower, and cannot use glBindTextureUnit, we have to do the following two lines below
		// Essentially how textures are working with the batch renderer system, now.
		glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, id); // Instead of using this we have to use glBindTexture on Mac
    }

    ///////////////////////////////////////////////
    ///////////////////////////////////////////////
    ///////////////////////////////////////////////
    ///////////////////////////////////////////////
    // Texture3D
    ///////////////////////////////////////////////

    OpenGLTexture3D::OpenGLTexture3D(uint32_t w, uint32_t h) : width(w), height(h){
        internalFormat = GL_RGBA8;
        formatData = GL_RGBA;

        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		// Since we segfault using glTextureParameteri, we'll use this for now
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    OpenGLTexture3D::OpenGLTexture3D(const std::string& filename) : filepath(filename) {
        RENDER_PROFILE_FUNCTION();

        coreLogInfo("OpenGLTexture2D Create Called!");
        int imageWidth, imageHeight, channels;

        stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = nullptr;
		{
		RENDER_PROFILE_SCOPE("stbi_load - OpenGLTexture2D::OpenGLTexture2D(const std::string&)");
        // Loading in our image data
        data = stbi_load(filepath.c_str(), &imageWidth, &imageHeight, &channels, 0);
		}

        if(data == nullptr){
            coreLogError("Failed to load image as the value was nullptr!");
        }
        else coreLogInfo("Image Data was not nullptr meaning it was valid!");
        // render_core_assert(data, "Failed to load image!");

        width = imageWidth;
        height = imageHeight;

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

		internalFormat = internalData, this->formatData = formatData;

        // Testing textures for OpenGL
        glGenTextures(1, &id); // Equivalent to glCreateTexture (but will segfault though)
        glActiveTexture(GL_TEXTURE_3D);
        glBindTexture(GL_TEXTURE_3D, id);
		// /* glCreateTextures(GL_TEXTURE_3D, 1, &_rendererID); */
		// /* glTextureStorage2D(GL_TEXTURE_3D, 0, _internalFormat, _width, _height); */
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		// Since we segfault using glTextureParameteri, we'll use this for now
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		/* glTextureParameteri(_rendererID, GL_TEXTURE_WRAP_S, GL_REPEAT); */
		/* glTextureParameteri(_rendererID, GL_TEXTURE_WRAP_T, GL_REPEAT); */

        glTexImage2D(GL_TEXTURE_3D, 0, internalData, width, height, 0, formatData, GL_UNSIGNED_BYTE, data); // same thing as doing: glTextureSubImage2D

        glGenerateMipmap(GL_TEXTURE_3D);

		stbi_image_free(data); // free image data stored in CPU

        glBindTexture(GL_TEXTURE_3D, 0);
    }

    void OpenGLTexture3D::bind(GLenum slot) const{
        RENDER_PROFILE_FUNCTION();

        /* glBindTextureUnit(slot, _rendererID); // Equivalent to glBindTexture (but will segfault though) */

		// Since we are using OpenGL 4.5 or lower, and cannot use glBindTextureUnit, we have to do the following two lines below
		// Essentially how textures are working with the batch renderer system, now.
		glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_3D, id); // Instead of using this we have to use glBindTexture on Mac
    }

    void OpenGLTexture3D::SetDataInternal(void* data, uint32_t size){
        /* glTextureSubImage2D(_rendererID, 0, 0, 0, _width, _height, */ 
		
		// bytes per pixel
		uint32_t bpp = formatData == GL_RGBA ? 4 : 3;
		// render_core_assert(size == _width * _height * bpp, "Data must be an entire texture"); // Make sure that the textures are fine for rn
        glTexImage2D(GL_TEXTURE_3D, 0, internalFormat, width, height, 0, formatData, GL_UNSIGNED_BYTE, data); // same thing as doing: glTextureSubImage2D
    }

    ///////////////////////////////////////////////
    ///////////////////////////////////////////////
    ///////////////////////////////////////////////
    ///////////////////////////////////////////////
    // Customizable Texture
    ///////////////////////////////////////////////
    template<GLenum T>
    OpenGLCustomizedTexture<T>::OpenGLCustomizedTexture(uint32_t w, uint32_t h) : width(w), height(h){
        internalFormat = GL_RGBA8;
        formatData = GL_RGBA;

        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		// Since we segfault using glTextureParameteri, we'll use this for now
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    template<GLenum T>
    OpenGLCustomizedTexture<T>::OpenGLCustomizedTexture(const std::string& filename) : filepath(filename) {
        RENDER_PROFILE_FUNCTION();

        coreLogInfo("OpenGLTexture2D Create Called!");
        int imageWidth, imageHeight, channels;

        stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = nullptr;
		{
		RENDER_PROFILE_SCOPE("stbi_load - OpenGLTexture2D::OpenGLTexture2D(const std::string&)");
        // Loading in our image data
        data = stbi_load(filepath.c_str(), &imageWidth, &imageHeight, &channels, 0);
		}

        if(data == nullptr){
            coreLogError("Failed to load image as the value was nullptr!");
        }
        else coreLogInfo("Image Data was not nullptr meaning it was valid!");
        // render_core_assert(data, "Failed to load image!");

        width = imageWidth;
        height = imageHeight;

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

		internalFormat = internalData, this->formatData = formatData;

        // Testing textures for OpenGL
        glGenTextures(1, &id); // Equivalent to glCreateTexture (but will segfault though)
        glActiveTexture(GL_TEXTURE_3D);
        glBindTexture(GL_TEXTURE_3D, id);
		// /* glCreateTextures(GL_TEXTURE_3D, 1, &_rendererID); */
		// /* glTextureStorage2D(GL_TEXTURE_3D, 0, _internalFormat, _width, _height); */
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		// Since we segfault using glTextureParameteri, we'll use this for now
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		/* glTextureParameteri(_rendererID, GL_TEXTURE_WRAP_S, GL_REPEAT); */
		/* glTextureParameteri(_rendererID, GL_TEXTURE_WRAP_T, GL_REPEAT); */

        glTexImage2D(GL_TEXTURE_3D, 0, internalData, width, height, 0, formatData, GL_UNSIGNED_BYTE, data); // same thing as doing: glTextureSubImage2D

        glGenerateMipmap(GL_TEXTURE_3D);

		stbi_image_free(data); // free image data stored in CPU

        glBindTexture(GL_TEXTURE_3D, 0);
    }

    template<GLenum T>
    void OpenGLCustomizedTexture<T>::bind(GLenum slot) const{
        RENDER_PROFILE_FUNCTION();

        /* glBindTextureUnit(slot, _rendererID); // Equivalent to glBindTexture (but will segfault though) */

		// Since we are using OpenGL 4.5 or lower, and cannot use glBindTextureUnit, we have to do the following two lines below
		// Essentially how textures are working with the batch renderer system, now.
		glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_3D, id); // Instead of using this we have to use glBindTexture on Mac
    }

    template<GLenum T>
    void OpenGLCustomizedTexture<T>::SetDataInternal(void* data, uint32_t size){
        /* glTextureSubImage2D(_rendererID, 0, 0, 0, _width, _height, */ 
		
		// bytes per pixel
		uint32_t bpp = formatData == GL_RGBA ? 4 : 3;
		// render_core_assert(size == _width * _height * bpp, "Data must be an entire texture"); // Make sure that the textures are fine for rn
        glTexImage2D(GL_TEXTURE_3D, 0, internalFormat, width, height, 0, formatData, GL_UNSIGNED_BYTE, data); // same thing as doing: glTextureSubImage2D
    }


};
