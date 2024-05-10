#include <Engine3D/Engine3DPrecompiledHeader.h>
// #include "interfaces/RendererAPI.h"
#include <Engine3D/Graphics/Framebuffer.h>
#include <Engine3D/OpenGL/OpenGLFramebuffer.h>
/* #include <Engine3D/platforms/OpenGL/OpenGLFramebuffe_Experiemental.h> */

namespace Engine3D {
	Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecifications &specs){
		switch (RendererAPI::getAPI()) {
			// case RendererAPI::API::None: render_core_assert(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case Engine3D::RendererAPI::API::None: assert(false);
			case RendererAPI::API::OpenGL: return CreateRef<OpenGLFrameBuffer>(specs);
		}

		// render_core_assert(false, "Unknown RendererAPI!");
		assert(false);
		return nullptr;
	}
}
