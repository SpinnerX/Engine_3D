#include <GameEnginePrecompiledHeader.h>
#include <GameEngine/Renderer/RenderCommand.h>
#include <GameEngine/platforms/OpenGL/OpenGLRendererAPI.h> // OpenGL draw calls are going to be implemented

namespace RendererEngine{
    RendererAPI* RendererCommand::_rendererAPI = new OpenGLRendererAPI;
};