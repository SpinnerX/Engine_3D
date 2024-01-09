#pragma once
#include <GameEngine/Renderer/Texture.h>
#include <cstdint>
#include <string>

namespace RendererEngine{

    // Texture2D - opengl-specific implementations for textures
    /** @note keep filepath - keeping filepath letting the development of the engine known when filepaths been modified, etc. */
    class OpenGLTexture2D : public Texture2D{
    public:
		OpenGLTexture2D(uint32_t width, uint32_t height);
        OpenGLTexture2D(const std::string& filepath);

        virtual ~OpenGLTexture2D();

        virtual uint32_t getWidth() const override { return _width; }
        virtual uint32_t getHeight() const override { return _height; }

		virtual void setData(void* data, uint32_t size) override;

        virtual void bind(GLenum bind = GL_TEXTURE_2D) const override; 

    private:
        std::string _filepath;
        uint32_t _width;
        uint32_t _height;
        uint32_t _rendererID;
		GLenum _internalFormat, _formatData;
    };

};
