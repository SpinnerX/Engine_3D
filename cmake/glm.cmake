# glm is basically an opengl math library for graphics programming
# API page for glm link: http://glm.g-truc.net/0.9.9/api/modules.html
set(GLM_DIR /opt/homebrew/include/glm)

if(EXISTS ${GLM_DIR})
    message(STATUS "Glm in ${GLM_DIR} has been found")
    find_package(glm REQUIRED)
    target_link_libraries(${PROJECT_NAME} glm::glm)
else()
    message(SEND_ERROR "\n===== [CMAKE] ===== >>> Glm in ${GLM_DIR} has not been found\n")
    if(APPLE)
        message(SEND_ERROR "\n===== [CMAKE] ===== >>> Since your on Mac you need to install via homebrew!!!\n")
        message(SEND_ERROR "\n===== [CMAKE] ===== >>> Since your on Windows you need to install via going to GLM link to install it, and thhen set the directory before find_package!!!\n")
    elseif(WIN32) # Give us error message if we are on Windows
        message(SEND_ERROR "\n===== [CMAKE] ===== >>> Since your on Windows you need to install via going to GLM link to install it, and thhen set the directory before find_package!!!\n")
        message(SEND_ERROR "\nOnce you installed it from the website then set the directory to ==> set(glm_DIR <installation prefix>/lib/cmake/glm)\n")
    endif(APPLE)
endif()

