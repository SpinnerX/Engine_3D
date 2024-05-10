#pragma once
#include <Engine3D/Graphics/Texture.h>
#include <cstdint>
#include <string>

namespace Engine3D{

    // Texture2D - opengl-specific implementations for textures
    /** @note keep filepath - keeping filepath letting the development of the engine known when filepaths been modified, etc. */
    class OpenGLTexture2D : public Texture2D{
    public:
		OpenGLTexture2D(uint32_t width, uint32_t height);
        OpenGLTexture2D(const std::string& filepath);
        virtual ~OpenGLTexture2D();

    private:
		uint32_t GetRendererIDInternal() const override { return id; }

        uint32_t GetWidthInternal() const override { return width; }
        uint32_t GetHeightInternal() const override { return height; }

		void SetDataInternal(void* data, uint32_t size) override;

        void bind(GLenum bind = GL_TEXTURE_2D) const override;

		bool operator==(const Texture2D& other) const override{
			return id == ((OpenGLTexture2D &)other).id;
		}

    private:
        std::string filepath;
        uint32_t width, height;
        uint32_t id;
		GLenum internalFormat, formatData;
    };


    class OpenGLTexture3D : public Texture3D {
    public:
        OpenGLTexture3D(uint32_t width, uint32_t height);
        OpenGLTexture3D(const std::string& filepath);

        uint32_t GetRendererIDInternal() const override { return id; }

        uint32_t GetWidthInternal() const override { return width; }
        uint32_t GetHeightInternal() const override { return height; }

		void SetDataInternal(void* data, uint32_t size) override;

        void bind(GLenum bind = GL_TEXTURE_3D) const override;

		bool operator==(const Texture3D& other) const override{
			return id == ((OpenGLTexture3D&)other).id;
		}

    private:
        std::string filepath;
        uint32_t width;
        uint32_t height;
        uint32_t id;
		GLenum internalFormat, formatData;
    };

    template<GLenum type>
    class OpenGLCustomizedTexture : public CustomizedTexture<type> {
    public:
        OpenGLCustomizedTexture(uint32_t width, uint32_t height);
        OpenGLCustomizedTexture(const std::string& filepath);

        uint32_t GetRendererIDInternal() const override { return id; }

        uint32_t GetWidthInternal() const override { return width; }
        uint32_t GetHeightInternal() const override { return height; }

		void SetDataInternal(void* data, uint32_t size) override;

        void bind(GLenum bind = GL_TEXTURE_3D) const override;

		bool operator==(const CustomizedTexture<type>& other) const override{
			return id == ((OpenGLCustomizedTexture<type>&)other).id;
		}

    private:
        std::string filepath;
        uint32_t width;
        uint32_t height;
        uint32_t id;
		GLenum internalFormat, formatData;
    };

};
