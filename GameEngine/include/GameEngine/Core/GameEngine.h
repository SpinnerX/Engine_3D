#pragma once

/**
 * 
 * 
 * Only intended to be used and called by client applications
 * 
 * For use by GameEngine application
 * 
*/

#include <GameEngine/Core/Application.h>
#include <GameEngine/Core/Layer.h>

// ------------- Engine Logger ---------------
#include <GameEngine/Core/EngineLogger.h>
// -------------------------------------------

#include <GameEngine/Core/Timestep.h>

#include <GameEngine/Core/InputPoll.h>
#include <GameEngine/Core/KeyCodes.h>
#include <GameEngine/Core/MouseButtonCodes.h>

#include <GameEngine/Imgui/ImGuiLayer.h>

// Renderer-related stuff (virtual interfaces, impl's, etc)
#include <GameEngine/Renderer/Renderer.h>
#include <GameEngine/Renderer/Renderer2D.h>
#include <GameEngine/Renderer/RenderCommand.h>

#include <GameEngine/Renderer/Buffer.h>
#include <GameEngine/Renderer/Shader.h>
#include <GameEngine/platforms/OpenGL/OpenGLShader.h> // May delete this later, as is not needed...
#include <GameEngine/Renderer/Texture.h>
#include <GameEngine/Renderer/VertexArray.h>


#include <GameEngine/Renderer/Renderer.h>
#include <GameEngine/Renderer/OrthographicCamera.h>
#include <GameEngine/platforms/OpenGL/OpenGLVertexArray.h>
#include <GameEngine/platforms/OpenGL/OpenGLBuffer.h>

