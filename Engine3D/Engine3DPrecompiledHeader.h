#pragma once
#define _CRT_SECURE_NO_WARNINGS

#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif

#include <cstdarg>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <sstream>
#include <fstream>

#include <array>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <thread>

// OpenGL includes
#ifndef __gl__h
#include <glad/glad.h>
#endif
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include <fmt/ostream.h>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <ImGuizmo/ImGuizmo.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Engine3D/Image/stb_image.h>

#include <Engine3D/Core/Application.h>
#include <Engine3D/Core/EngineLogger.h>
#include <Engine3D/Event/InputPoll.h>
#include <Engine3D/Core/Timer.h>
#include <Engine3D/Core/Timestep.h>

#include <Engine3D/Event/Event.h>
#include <Engine3D/Event/KeyEvent.h>
#include <Engine3D/Event/MouseEvent.h>
#include <Engine3D/Event/ApplicationEvent.h>

// #include <Engine3D/Graphics/Buffer.h>
// #include <Engine3D/Graphics/Shader.h>
// #include <Engine3D/OpenGL/OpenGLShader.h>
// #include <Engine3D/Graphics/VertexArray.h>
// #include <Engine3D/Graphics/Texture.h>
#include <Engine3D/OpenGL/OpenGLContext.h>
#include <Engine3D/Graphics/RendererAPI.h>
#include <Engine3D/OpenGL/OpenGLShader.h>
#include <Engine3D/OpenGL/OpenGLTexture.h>
// #include <Engine3D/Graphics/Pipeline.h>
#include <Engine3D/Renderer2D/RenderCommand.h>
#include <Engine3D/Renderer2D/Renderer.h>
#include <Engine3D/Renderer2D/Renderer2D.h>

#include <Engine3D/Core/EngineLogger.h>
#include <Engine3D/Renderer2D/OrthographicCameraController.h>
#include <Engine3D/Debug/Instrumentor.h>

#include <Engine3D/Scene2D/Components.h>
#include <Engine3D/Scene2D/Entity.h>
#include <Engine3D/Scene2D/Scene.h>
#include <Engine3D/Scene2D/SceneSerializer.h>
#include <Engine3D/Scene2D/ScriptableEntity.h>
#include <Engine3D/Math/Math.h>

#include <Engine3D/platforms/PlatformUtils.h>

//! @note Physics-related includes
// @note box2D related includes
#include <box2d/box2d.h>
#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>

// #include <glm/glm.hpp>
// #include <Engine3D/Event/ApplicationEvent.h>

// #include <Engine3D/Core/EngineLogger.h>
// #include <Engine3D/Renderer/OrthographicCameraController.h>
// #include <Engine3D/Debug/Instrumentor.h>



#if ENGINE_PLATFORM_WINDOWS
    #include <Windows.h> // include Windows.h if we are in Windows platform
#endif