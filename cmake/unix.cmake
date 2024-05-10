if(APPLE)
list(APPEND all_headers ../${INCLUDE_DIR}/platforms/Mac/nfd.h ../${INCLUDE_DIR}/platforms/Mac/nfd.hpp)
endif(APPLE)

include(CMakeFindDependencyMacro)

# @note find_dependency is how you can specify what dependencies will be shipped with Engine3D
# @note When users try to find this packages.

# find_package(glfw3 REQUIRED)
find_dependency(glfw3 REQUIRED)

# find_package(OpenGL REQUIRED)
# find_package(GLEW REQUIRED)
find_dependency(OpenGL REQUIRED)
if(APPLE)
find_dependency(GLEW REQUIRED)
endif(APPLE)
# find_package(imgui REQUIRED)
# find_package(ImGuizmo REQUIRED)
find_dependency(imgui REQUIRED)
find_dependency(ImGuizmo REQUIRED)

# find_package(fmt REQUIRED)
# find_package(spdlog REQUIRED)
# find_package(glad REQUIRED)
# find_package(glm REQUIRED)

find_dependency(fmt REQUIRED)
find_dependency(spdlog REQUIRED)
find_dependency(glad REQUIRED)
find_dependency(glm REQUIRED)

find_dependency(box2d REQUIRED)
find_dependency(yaml-cpp REQUIRED)


# NOTE: Using {OPENGL_gl_LIBRARY}, if compiling on Apple.

if(UNIX AND NOT APPLE)
	target_link_libraries(
		${PROJECT_NAME}
		glfw
		# ${OPENGL_gl_LIBRARY}
		OpenGL::GL
		# Vulkan::Vulkan
		imgui::imgui
		fmt::fmt
		glm::glm
		spdlog::spdlog
		glad::glad
		box2d::box2d
		yaml-cpp::yaml-cpp
	)
endif(UNIX AND NOT APPLE)

if(APPLE)
target_link_libraries(
	${PROJECT_NAME}
	PUBLIC
	glfw
	"-framework Cocoa"
	"-framework OpenGL"
	"-framework IOKit"
	${APPKIT_LIBRARY} ${UNIFORMTYPEIDENTIFIERS_LIBRARY} # Note as TODO: Probably should put this somewhere else, since it relies on nfd.cmake...
	imgui::imgui
	ImGuizmo::ImGuizmo
	fmt::fmt
	glm::glm
	spdlog::spdlog
	glad::glad
	GLEW::GLEW
	box2d::box2d
	yaml-cpp::yaml-cpp
	${APPKIT_LIBRARY} ${UNIFORMTYPEIDENTIFIERS_LIBRARY} # Note as TODO: Probably should put this somewhere else, since it relies on nfd.cmake...
)
endif(APPLE)