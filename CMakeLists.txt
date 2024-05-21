cmake_minimum_required(VERSION 3.15)

set(PROJECT_NAME Engine3D)
set(CMAKE_CXX_STANDARD 20)

if(APPLE)
add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)
endif(APPLE)

project(${PROJECT_NAME} VERSION 1.0)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "") # works (in creating the compile_commands.json file)

option(ENGINE_BUILD_UNIT_TESTS "Build the GameEngine unit tests" ON)
option(ENGINE_BUILD_TESTBED "Build the GameEngine testbed" ON)
option(ENGINE_BUILD_DOCS "Build the GameEngine documentation" OFF)
option(ENGINE_USER_SETTINGS "Override GameEngine settings with GameEngineUserSettings.h" OFF)

option(BUILD_SHARED_LIBS "Build Box2D as a shared library" OFF)

include(GNUInstallDirs)
include_directories(
    ../
	${PROJECT_SOURCE_DIR}/Engine3D
)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

message("Message = ${CMAKE_INSTALL_PREFIX}")

# add_subdirectory(Sandbox)
# add_subdirectory(Sandbox)
add_subdirectory(src)


# target_precompile_headers(${PROJECT_NAME} PUBLIC
# 	"$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_INSTALL_PREFIX}/include/GameEngine/GameEnginePrecompiledHeader.h>"
# )

set(PRECOMPILED_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Engine3D/Engine3DPrecompiledHeader.h>")
set(INSTALLED_PRECOMPILED_PATH "${CMAKE_INSTALL_PREFIX}/Engine3D/Engine3DPrecompiledHeader.h>")

if(EXISTS ${INSTALLED_PRECOMPILED_PATH})
	set(PATH ${INSTALLED_PRECOMPILED_PATH})
elseif(EXISTS ${PRECOMPILED_PATH})
	set(PATH ${PRECOMPILED_PATH})
endif()

file(COPY Resources DESTINATION ${CMAKE_BINARY_DIR})

# Before we copy compile_commands.json to .vscode
# Making sure we should make sure if that dir already exists in the root of the projects directory
set(VSCODE_DIR "${CMAKE_CURRENT_LIST_DIR}/.vscode")
if(NOT EXISTS ${VSCODE_DIR})
    file(MAKE_DIRECTORY ${VSCODE_DIR})
endif()

# Copy to source directory
add_custom_target(
    copy-compile-commands ALL
    DEPENDS
    ${CMAKE_CURRENT_LIST_DIR}/compile_commands.json
)

# Creating a customize command specific for copying the compile_commands.json to users .vscode directory
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_LIST_DIR}/compile_commands.json
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_BINARY_DIR}/compile_commands.json
        ${CMAKE_CURRENT_LIST_DIR}/.vscode/compile_commands.json
    DEPENDS
    # Unlike "proper" targets like executables and libraries, 
    # custom command / target pairs will not set up source
    # file dependencies, so we need to list file explicitly here
    generate-compile-commands
    ${CMAKE_BINARY_DIR}/compile_commands.json
)

# Generate the compilation commands. Necessary so cmake knows where it came
# from and if for some reason you delete it.
add_custom_target(generate-compile-commands
    DEPENDS
        ${CMAKE_BINARY_DIR}/compile_commands.json
)

# add_custom_command(
#     OUTPUT ${CMAKE_BINARY_DIR}/compile_commands.json
#     COMMAND ${CMAKE_COMMAND} -B${CMAKE_BINARY_DIR} -S${CMAKE_SOURCE_DIR}
# )


# file(COPY Resources DESTINATION ${CMAKE_BINARY_DIR})
# file(COPY Resources DESTINATION ${CMAKE_INSTALL_PREFIX}/Engine3D)
# file(COPY ${CMAKE_SOURCE_DIR}/Resources DESTINATION /usr/local/public/Engine3D)


if(WIN32)
include(cmake/win32.cmake)
elseif(UNIX)
include(cmake/unix.cmake)
endif()

target_precompile_headers(${PROJECT_NAME} PUBLIC
	"$<$<COMPILE_LANGUAGE:CXX>:>"
)

install(
	DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/Engine3D"
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Installing to /usr/local/public/GameEngine/
# install(
#   DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/Resources"
#   DESTINATION "${CMAKE_INSTALL_LIBDIR}/GameEngine"
# )
