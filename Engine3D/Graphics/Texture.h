#pragma once
#include <Engine3D/Core/core.h>
#include <GLFW/glfw3.h>
#include <string>

namespace Engine3D{


    /**
     * @brief 
     * 
     * @class Texture (class) - Pure virtual class representing the texture
     * @class Texture2D - Represents our actual 2D texture pure virtual class interface for different platforms to impl
     */
    // class Texture{
    // public:
    //     virtual ~Texture() = default;
    // private:
    //     virtual uint32_t getWidth() const = 0;
    //     virtual uint32_t getHeight() const = 0;
		
	// 	virtual uint32_t getRendererID() const = 0;

	// 	// @setData simply will take in a block of data in memory, and upload that data to the GPU
	// 	virtual void setData(void* data, uint32_t size) = 0;

    //     virtual void bind(GLenum slot=GL_TEXTURE_2D) const = 0;

	// 	virtual bool operator==(const Texture& other) const = 0;
    // };

    template<GLenum type>
    class CustomizedTexture{
    public:
        static Ref<CustomizedTexture> Create(uint32_t width, uint32_t height);
        static Ref<CustomizedTexture> Create(const std::string& filename);

        void Bind(GLenum slot=type) const;
        uint32_t GetWidth() const;
        uint32_t GetHeight() const;

        void SetData(void* data, uint32_t size);

        uint32_t GetRendererID() const;

        virtual bool operator==(const CustomizedTexture& other) const = 0;


    private:
        virtual uint32_t GetWidthInternal() const = 0;
        virtual uint32_t GetHeightInternal() const = 0;
		
		virtual uint32_t GetRendererIDInternal() const = 0;

		// @setData simply will take in a block of data in memory, and upload that data to the GPU
		virtual void SetDataInternal(void* data, uint32_t size) = 0;

        virtual void bind(GLenum slot=GL_TEXTURE_2D) const = 0;
    };


    class Texture2D{
    public:
        virtual ~Texture2D() = default;
        static Ref<Texture2D> Create(uint32_t width, uint32_t height);
        static Ref<Texture2D> Create(const std::string& filepath);

        void Bind(GLenum slot=GL_TEXTURE_2D) const;
        uint32_t GetWidth() const;
        uint32_t GetHeight() const;

        void SetData(void* data, uint32_t size);

        uint32_t GetRendererID() const;

        virtual bool operator==(const Texture2D& other) const = 0;
    private:
        virtual uint32_t GetWidthInternal() const = 0;
        virtual uint32_t GetHeightInternal() const = 0;
		
		virtual uint32_t GetRendererIDInternal() const = 0;

		// @setData simply will take in a block of data in memory, and upload that data to the GPU
		virtual void SetDataInternal(void* data, uint32_t size) = 0;

        virtual void bind(GLenum slot=GL_TEXTURE_2D) const = 0;
    };

    class Texture3D{
    public:
        virtual ~Texture3D() = default;
        static Ref<Texture3D> Create(uint32_t width, uint32_t height);
        static Ref<Texture3D> Create(const std::string& name);

        uint32_t GetRendererID() const;

        void Bind(GLenum slot=GL_TEXTURE_3D) const;
        uint32_t GetWidth() const;
        uint32_t GetHeight() const;

        void SetData(void* data, uint32_t size);

        virtual bool operator==(const Texture3D& other) const = 0;

    private:
        virtual uint32_t GetWidthInternal() const = 0;
        virtual uint32_t GetHeightInternal() const = 0;
		
		virtual uint32_t GetRendererIDInternal() const = 0;

		// @setData simply will take in a block of data in memory, and upload that data to the GPU
		virtual void SetDataInternal(void* data, uint32_t size) = 0;

        virtual void bind(GLenum slot=GL_TEXTURE_2D) const = 0;
    };
};
