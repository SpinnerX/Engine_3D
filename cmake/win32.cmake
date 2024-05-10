
message("Running Win32 CMake Build...")

set(VULKAN_ROOT "C:\\VulkanSDK\\1.3.280.0")
set(VULKAN_INCLUDE "${VULKAN_ROOT}\\Include")

if (EXISTS ${VULKAN_ROOT})
include_directories(${VULKAN_INCLUDE})
list(APPEND CMAKE_PREFIX_PATH "C:\\VulkanSDK\\1.3.280.0\\Bin")
endif()

include(CMakeFindDependencyMacro)
find_package(Vulkan REQUIRED)


find_dependency(OpenGL REQUIRED )

# include_directories( ${OPENGL_INCLUDE_DIRS} external/ImGuizmo/include external/box2d/include
include_directories( ${OPENGL_INCLUDE_DIRS} external/box2d/include)

set( GLFW_BUILD_DOCS OFF CACHE BOOL  "GLFW lib only" )
set( GLFW_INSTALL OFF CACHE BOOL  "GLFW lib only" )
set( GLAD_GL "" )
add_subdirectory(external/glfw)

option( GLFW-CMAKE-STARTER-USE-GLFW-GLAD "Use GLAD from GLFW" ON )

if( GLFW-CMAKE-STARTER-USE-GLFW-GLAD )
    include_directories("${GLFW_SOURCE_DIR}/deps")
    set( GLAD_GL "${GLFW_SOURCE_DIR}/deps/glad/gl.h" )
endif()

if( MSVC )
    SET( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:mainCRTStartup" )
endif()

add_subdirectory(external/glm)
add_subdirectory(external/yaml-cpp)

add_subdirectory(external/Box2D-cmake)

include(FetchContent)
add_subdirectory(external/fmt)


add_subdirectory(external/spdlog)

add_subdirectory(external/imgui)

add_subdirectory(external/glad)

target_link_libraries(${PROJECT_NAME}
    ${OPENGL_LIBRARIES}
    glfw
    ${Vulkan_LIBRARIES}
    fmt::fmt
    spdlog::spdlog
    glm::glm
    tobanteGaming::Box2D
    yaml-cpp::yaml-cpp
    imgui
	glad
)