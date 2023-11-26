cmake_minimum_required(VERSION 3.15)
set(PROJECT_NAME GameEngine)
set(CMAKE_CXX_FLAGS "-Wno-dev")
set(CMAKE_CXX_FLAGS "-std=c++20")

project(${PROJECT_NAME})
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wno-dev")
set(CMAKE_CXX_FLAGS "-fno-exceptions" "-fsanitize=address" "-Wall" "-Wextra")
set(CMAKE_CXX_FLAGS "-std=c++20")

add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)

set(CMAKE_BUILT_TYPE Debug)
set(THREADS_PREFER_PTHREAD_FLAG ON)

include(cmake/precompiled_headers.cmake)

find_package(fmt REQUIRED)
find_package(spdlog REQUIRED)
find_package(Threads REQUIRED)
include(cmake/imgui.cmake)
include(cmake/glad.cmake)

include_directories(
  ${PROJECT_SOURCE_DIR}/GameEngine/include
  ${PROJECT_SOURCE_DIR}/GameEngine/include/GameEngine
  ${imgui_include}
  ${glad_include}
)

set(
  all_src
  GameEngine/src/GameEngine/core.cpp
  GameEngine/include/GameEngine/Application.cpp
  GameEngine/include/GameEngine/EngineLogger.cpp
  GameEngine/src/GameEngine/platforms/WindowsWindow.cpp
  GameEngine/include/GameEngine/Layer.cpp
  GameEngine/include/GameEngine/LayerStack.cpp
  ${imgui_src}
  ${glad_src}

  GameEngine/include/GameEngine/Imgui/ImGuiLayer.cpp
  GameEngine/include/GameEngine/InputPoll.cpp

  GameEngine/src/GameEngine/platforms/WindowsInput.cpp
  GameEngine/src/GameEngine/Renderer/RendererContext.cpp
  GameEngine/include/GameEngine/Renderer/Shader.cpp
  GameEngine/include/GameEngine/Renderer/Buffer.cpp
  GameEngine/include/GameEngine/platforms/OpenGL/OpenGLBuffer.cpp
  GameEngine/include/GameEngine/Renderer/Renderer.cpp
  GameEngine/include/GameEngine/Renderer/VertexArray.cpp
  GameEngine/include/GameEngine/platforms/OpenGL/OpenGLVertexArray.cpp
  GameEngine/include/GameEngine/Renderer/RendererAPI.cpp
  GameEngine/include/GameEngine/Renderer/RendererCommand.cpp
  GameEngine/include/GameEngine/platforms/OpenGL/OpenGLRendererAPI.cpp
)

add_precompiled_header(${PROJECT_NAME} include/GameEnginePrecompiledHeader.h FORCEINCLUDE SOURCE_CXX src/GameEnginePrecompiledHeader.cpp)


add_subdirectory(Sandbox)

add_library(${PROJECT_NAME} SHARED ${all_src})
add_library(Engine::Engine ALIAS ${PROJECT_NAME}) # Allows to alias the library

set_property(TARGET ${PROJECT_NAME} PROPERTY INTERPROCEDURAL_OPTIMIZATION True)

# Linking the included directories
target_include_directories(
  ${PROJECT_NAME}
  PUBLIC
  ${PROJECT_SOURCE_DIR}/GameEngine/include
  ${PROJECT_SOURCE_DIR}/GameEngine/include/GameEngine
)

# This is how you can have precompiled headers
# This allows for faster compile time
target_precompile_headers(${PROJECT_NAME} PRIVATE
  "$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_SOURCE_DIR}/GameEngine/include/GameEnginePrecompiledHeader.h>"
)
include(cmake/glm.cmake)
include(cmake/glfw.cmake)
target_link_libraries(${PROJECT_NAME} fmt::fmt spdlog::spdlog Threads::Threads)