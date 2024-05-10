#include <Engine3D/Engine3DPrecompiledHeader.h>
#include <Engine3D/Renderer2D/RenderCommand.h>
#include <Engine3D/OpenGL/OpenGLRendererAPI.h>

namespace Engine3D{
    Scope<RendererAPI> RendererCommand::_rendererAPI = CreateScope<OpenGLRendererAPI>();
};
