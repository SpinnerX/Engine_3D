# Ref Link: https://stackoverflow.com/questions/56682058/opengl-is-present-in-the-mingw-folder-but-clion-cant-reach-see-it


# Opengl cmake that makes sure we have glfw, glew, glut, and glad installed.

############################################################################
############ [ CMAKE MESSAGE-RELATED COLORED TEXT] #########################
############################################################################
# This is to allow coloring text
if(NOT WIN32)
  string(ASCII 27 Esc)
  set(ColourReset "${Esc}[m")
  set(ColourBold  "${Esc}[1m")
  set(Red         "${Esc}[31m")
  set(Green       "${Esc}[32m")
  set(Yellow      "${Esc}[33m")
  set(Blue        "${Esc}[34m")
  set(Magenta     "${Esc}[35m")
  set(Cyan        "${Esc}[36m")
  set(White       "${Esc}[37m")
  set(BoldRed     "${Esc}[1;31m")
  set(BoldGreen   "${Esc}[1;32m")
  set(BoldYellow  "${Esc}[1;33m")
  set(BoldBlue    "${Esc}[1;34m")
  set(BoldMagenta "${Esc}[1;35m")
  set(BoldCyan    "${Esc}[1;36m")
  set(BoldWhite   "${Esc}[1;37m")
endif()

############################################################################
############################################################################

# Finding opengl, glfw, glew, glut, and possibly freetype and even freeglut
find_package(glfw3 3.4 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLUT REQUIRED)
find_package(FREEGLUT REQUIRED)

# Checking if glfw3 has been found meaning installed
if(NOT TARGET glfw)
    # message("GLFW HAS BEEN FOUND - NOT THE CMAKE MESSAGE BY YOUR MESSAGE!\n\n\n")
    message("glfw.cmake ERROR MESSAGE - glfw3 has not been found!")
    message("glfw.cmake ERROR MESSAGE - you should check if glfw has been installed!\n")
endif()


# Checking if OPENGL/GLEW/GLUT is found meaning installed
if(NOT OPENGL_FOUND)
    message("${Red}NOT CMAKE ACTUAL MESSAGE --> OPENGL NOT FOUND!\n\n")
endif()

if(NOT GLEW_FOUND)
    message("${Red}NOT CMAKE ACTUAL MESSAGE --> GLEW NOT FOUND!\n\n")
endif()

if(NOT GLUT_FOUND)
    message("${Red}NOT CMAKE ACTUAL MESSAGE --> GLUT NOT FOUND!\n\n")
endif()




if(__APPLE__)

    # This is making sure we find the SDL libraries on mac
    include_directories(${SDL2_INCLUDE_DIRS})


    target_link_libraries(${PROJECT_NAME} pthread)
    # Linking the libraries for Cocoa, OpenGL, IOKit, and glfw
    # Need this for opengl
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
endif(__APPLE__)


target_link_libraries(${PROJECT_NAME}
    glfw
    ${OPENGL_gl_LIBRARY}
    GLEW::GLEW
    ${GLUT_LIBRARIES} # NOTE: If you want to use freeglut, then you need this to link freeglut to get working
)