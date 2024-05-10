#include <Engine3D/Engine3DPrecompiledHeader.h>
#include <Engine3D/Graphics/UniformBuffer.h>

namespace Engine3D{
	Ref<UniformBuffer> UniformBuffer::create(uint32_t size, uint32_t binding){
		switch (RendererAPI::getAPI()) {
			case RendererAPI::API::None: assert(false);
			case RendererAPI::API::OpenGL: break;
			default:
				assert(false);
		}
		return nullptr;
	}
};
