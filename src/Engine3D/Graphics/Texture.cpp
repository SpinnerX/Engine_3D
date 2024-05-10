#include <Engine3D/Engine3DPrecompiledHeader.h>
#include <Engine3D/Graphics/Texture.h>

namespace Engine3D{

    Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height){
		switch (RendererAPI::getAPI()) {
			case RendererAPI::API::None:
				// render_assert(false, "RendererAPI::None is currently not supported!");
                assert(false);
				return nullptr;
			case RendererAPI::API::OpenGL:
				return CreateRef<OpenGLTexture2D>(width, height);
		}
	}

    Ref<Texture2D> Texture2D::Create(const std::string& filepath){
        switch (RendererAPI::getAPI()){
        case RendererAPI::API::None:
            // render_core_assert(false, "RenderAPI::None is currently not supported!");
            assert(false);
            return nullptr;
        case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLTexture2D>(filepath);
        }

        // render_core_assert(false, "Unknown renderer API");
        assert(false);
        return nullptr;
    }

    void Texture2D::Bind(GLenum slot) const{
        bind(slot);
    }

    uint32_t Texture2D::GetRendererID() const{
        return GetRendererIDInternal();
    }

    uint32_t Texture2D::GetWidth() const { return GetWidthInternal(); }

    uint32_t Texture2D::GetHeight() const { return GetHeightInternal(); }

    void Texture2D::SetData(void* data, uint32_t size){
        SetDataInternal(data, size);
    }


    Ref<Texture3D> Texture3D::Create(uint32_t width, uint32_t height){
        switch (RendererAPI::getAPI()){
        case RendererAPI::API::OpenGL:
            return CreateRef<OpenGLTexture3D>(width, height);
        }

        assert(false);
        return nullptr;
    }

    Ref<Texture3D> Texture3D::Create(const std::string& name){
        switch(RendererAPI::getAPI()){
        case RendererAPI::API::OpenGL:
            return CreateRef<OpenGLTexture3D>(name);
        }

        assert(false);
        return nullptr;
    }

    void Texture3D::Bind(GLenum slot) const{
        bind(slot);
    }

    uint32_t Texture3D::GetRendererID() const{
        return GetRendererIDInternal();
    }

    uint32_t Texture3D::GetWidth() const { return GetWidthInternal(); }

    uint32_t Texture3D::GetHeight() const { return GetHeightInternal(); }

    void Texture3D::SetData(void* data, uint32_t size){
        SetDataInternal(data, size);
    }






    ///////////////////////////////////////////////
    ///////////////////////////////////////////////
    ///////////////////////////////////////////////
    ///////////////////////////////////////////////
    // Customizable Texture
    ///////////////////////////////////////////////

    template<GLenum T>
    Ref<CustomizedTexture<T>> CustomizedTexture<T>::Create(uint32_t width, uint32_t height){
        switch (RendererAPI::getAPI()){
        case RendererAPI::API::OpenGL:
            return CreateRef<OpenGLCustomizedTexture<T>>(width, height);
        }

        assert(false);
        return nullptr;
    }

    template<GLenum T>
    Ref<CustomizedTexture<T>> CustomizedTexture<T>::Create(const std::string& name){
        switch(RendererAPI::getAPI()){
        case RendererAPI::API::OpenGL:
            return CreateRef<OpenGLCustomizedTexture<T>>(name);
        }

        assert(false);
        return nullptr;
    }

    template<GLenum T>
    void CustomizedTexture<T>::Bind(GLenum slot) const{
        bind(slot);
    }

    template<GLenum T>
    uint32_t CustomizedTexture<T>::GetRendererID() const{
        return GetRendererIDInternal();
    }

    template<GLenum T>
    uint32_t CustomizedTexture<T>::GetWidth() const { return GetWidthInternal(); }

    template<GLenum T>
    uint32_t CustomizedTexture<T>::GetHeight() const { return GetHeightInternal(); }

    template<GLenum T>
    void CustomizedTexture<T>::SetData(void* data, uint32_t size){
        SetDataInternal(data, size);
    }


};
