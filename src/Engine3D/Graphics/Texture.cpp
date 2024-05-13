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

    bool Texture2D::IsLoaded() const { return IsTextureLoadedSuccessful(); }

    void Texture2D::SetData(void* data, uint32_t size){ SetDataInternal(data, size); }
};
