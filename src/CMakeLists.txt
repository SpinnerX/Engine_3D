include_directories(
	Engine3D
	../
)

set(
	INCLUDE_DIR
	Engine3D	
)

set(
  all_src


  Engine3D/Core/EntryPoint.cpp
  Engine3D/Core/Application.cpp
  Engine3D/Core/EngineLogger.cpp
  Engine3D/Core/Layer.cpp
  Engine3D/Core/LayerStack.cpp

  Engine3D/Imgui/ImGuiLayer.cpp

  Engine3D/OpenGL/OpenGLContext.cpp
  Engine3D/Graphics/Shader.cpp
  Engine3D/Graphics/Buffer.cpp
  Engine3D/OpenGL/OpenGLBuffer.cpp
  Engine3D/Renderer2D/Renderer.cpp
  Engine3D/Graphics/VertexArray.cpp
  Engine3D/OpenGL/OpenGLVertexArray.cpp
  Engine3D/Graphics/RendererAPI.cpp
  Engine3D/Renderer2D/RendererCommand.cpp
  Engine3D/OpenGL/OpenGLRendererAPI.cpp
  Engine3D/Renderer2D/OrthographicCamera.cpp
  Engine3D/OpenGL/OpenGLShader.cpp
  Engine3D/Graphics/Texture.cpp
  Engine3D/OpenGL/OpenGLTexture.cpp
  Engine3D/Renderer2D/OrthographicCameraController.cpp

  # Engine3D/OpenGL/OpenGLInputPoll.cpp
  Engine3D/Event/InputPoll.cpp
  
  Engine3D/OpenGL/OpenGLWindow.cpp
  Engine3D/Renderer2D/Renderer2D.cpp
  Engine3D/Graphics/SubTexture2D.cpp
  Engine3D/Graphics/Framebuffer.cpp
  Engine3D/OpenGL/OpenGLFramebuffer.cpp
  Engine3D/Scene2D/Scene.cpp
  Engine3D/Scene2D/Entity.cpp
  Engine3D/Scene2D/Components.cpp
  Engine3D/Scene2D/SceneCamera.cpp
  Engine3D/Scene2D/SceneSerializer.cpp
  Engine3D/Image/stb_image.cpp

  Engine3D/platforms/Mac/MacPlatformUtils.cpp
  Engine3D/Math/Math.cpp
  Engine3D/Renderer2D/EditorCamera.cpp
  Engine3D/Graphics/UniformBuffer.cpp
  Engine3D/OpenGL/OpenGLUniformBuffer.cpp
  Engine3D/Core/UUID.cpp
  Engine3D/Image/Image.cpp

)

# For Mac, linking AppKit only and only needing nfd_cocoa.m
if(APPLE)
  # For setting the filter list, macOS introduced allowedContentTypes in version 11.0 and deprecated allowedFileTypes in 12.0.
  # By default (set to ON), NFDe will use allowedContentTypes when targeting macOS >= 11.0.
  # Set this option to OFF to always use allowedFileTypes regardless of the target macOS version.
  # This is mainly needed for applications that are built on macOS >= 11.0 but should be able to run on lower versions
  # and should not be used otherwise.
  option(NFD_USE_ALLOWEDCONTENTTYPES_IF_AVAILABLE "Use allowedContentTypes for filter lists on macOS >= 11.0" ON)

  find_library(APPKIT_LIBRARY AppKit)
  if(NFD_USE_ALLOWEDCONTENTTYPES_IF_AVAILABLE)
    include(CheckCXXSourceCompiles)
    check_cxx_source_compiles(
      "
      #include <Availability.h>
      #if !defined(__MAC_OS_X_VERSION_MIN_REQUIRED) || !defined(__MAC_11_0) || __MAC_OS_X_VERSION_MIN_REQUIRED < __MAC_11_0
      static_assert(false);
      #endif
      int main() { return 0; }
      "
      NFD_USE_ALLOWEDCONTENTTYPES
    )
    if(NFD_USE_ALLOWEDCONTENTTYPES)
      find_library(UNIFORMTYPEIDENTIFIERS_LIBRARY UniformTypeIdentifiers)
      if(NOT UNIFORMTYPEIDENTIFIERS_LIBRARY)
        message(FATAL_ERROR "UniformTypeIdentifiers framework is not available even though we are targeting macOS >= 11.0")
      endif()
    endif()
  endif()
  list(APPEND all_src Engine3D/platforms/Mac/nfd_cocoa.m)
endif()

add_library( ${PROJECT_NAME} ${all_src})
