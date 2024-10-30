cmake_minimum_required (VERSION 3.15)

set(PROJECT_NAME Sandbox)

project(${PROJECT_NAME})

include_directories(
  ./
)
set(all_headers EditorLayer.h UI/UI.h Panels/SceneHeirarchyPanel.h)

set(all_src Sandbox.cpp EditorLayer.cpp UI/UI.cpp Panels/SceneHeirarchyPanel.cpp)

# Create the executable
add_executable(${PROJECT_NAME} ${all_headers} ${all_src})


# Log::Log1 is the alias we made when we created an alias in the Logger CMakeLists.txt file
target_link_libraries(
  ${PROJECT_NAME}
	Engine3D
  imguidocking::imguidocking
  imguizmo::imguizmo
)
