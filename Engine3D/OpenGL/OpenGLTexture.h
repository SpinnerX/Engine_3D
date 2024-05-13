#pragma once
#include <Engine3D/Graphics/Texture.h>
#include <cstdint>
#include <string>
#include <Engine3D/Image/Image.h>

namespace Engine3D{

    // Texture2D - opengl-specific implementations for textures
    /** @note keep filepath - keeping filepath letting the development of the engine known when filepaths been modified, etc. */
    class OpenGLTexture2D : public Texture2D{
    public:
		OpenGLTexture2D(uint32_t width, uint32_t height);
        OpenGLTexture2D(const std::string& filepath);
        virtual ~OpenGLTexture2D();

    private:
		uint32_t GetRendererIDInternal() const override;

        uint32_t GetWidthInternal() const override;
        uint32_t GetHeightInternal() const override;

        bool IsTextureLoadedSuccessful() const override;

		void SetDataInternal(void* data, uint32_t size) override;

        void bind(GLenum bind = GL_TEXTURE_2D) const override;

		bool operator==(const Texture2D& other) const override{
			return id == ((OpenGLTexture2D &)other).id;
		}

    private:
        std::string filepath;
        Image image;
        int width, height;

        // Ref<Image> image;
        uint32_t id;
		GLenum internalFormat, formatData;
        bool isTextureLoaded = false;
    };

};
