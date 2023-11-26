# NOTE (May also want to add this in for the other cmake modules as well)
# Make cmake be more platform specific (including support for Windows, Linux, and Mac)
# 1.) Setting up the filepath depending on the OS's (like windows)
# 2.) Checking these filepaths exist
# 3.) Depending on the OS is going to be how we set thhese filepaths
# 4.) Once we set them, then thhat is how we are going to link them.
if(APPLE)
    set(imgui_include "/usr/local/include/imgui")
elseif(WIN32)
    set(imgui_include "C:\\Desktop\\Libraries\\imgui")
endif(APPLE)

# NOTE
# We are checking if imgui has been found and moved to folder /usr/local/include as /usr/local/include/imgui
# If that directory is not found then go to the imgui github link and git clone and move those files into /usr/local/include
# Github Installation Link: https://github.com/ocornut/imgui

# NOTE ** in case you forget **
# - Uncomment some of the .cpp files if you installed everything correctly, and are having errors
# - Errors involving opengl2, dx v9, v12, v11, and v10, or using android, allegro and other interfaces
#   depending on OS and mobile requirements

# If I want to use ImGui's Docking stuff, I need to make sure the branch is changed from mastert to docking branch
# Branch for dock is called "docking"
# ** NOTE ** - Pretty weird, imo lol
# Ref Link: https://stackoverflow.com/questions/72807349/imgui-has-no-member-renderplatformwindowsdefault-error


# We want to check before proceeding
if(EXISTS ${imgui_include})
    message(STATUS "Imgui in ${imgui_include} has been found")
    set(
        imgui_src
        ${imgui_include}/imgui_draw.cpp
        ${imgui_include}/imgui_widgets.cpp
        ${imgui_include}/imgui_tables.cpp
        ${imgui_include}/imgui_demo.cpp
        ${imgui_include}/imgui.cpp


        # ${imgui_include}/backends/imgui_impl_opengl2.cpp
        # ${imgui_include}/backends/imgui_impl_glut.cpp
        ${imgui_include}/backends/imgui_impl_glfw.cpp
        ${imgui_include}/backends/imgui_impl_opengl3.cpp
        # ${imgui_include}/backends/imgui_impl_sdl2.cpp
        # ${imgui_include}/backends/imgui_impl_sdl3.cpp
        # ${imgui_include}/backends/imgui_impl_sdlrenderer2.cpp
        # ${imgui_include}/backends/imgui_impl_sdlrenderer3.cpp

        # ${imgui_include}/backends/imgui_impl_dx9.cpp
        # ${imgui_include}/backends/imgui_impl_dx12.cpp
        # ${imgui_include}/backends/imgui_impl_dx11.cpp
        # ${imgui_include}/backends/imgui_impl_dx10.cpp
        # ${imgui_include}/backends/imgui_impl_android.cpp
        # ${imgui_include}/backends/imgui_impl_allegro5.cpp

        # ${imgui_include}/backends/imgui_impl_vulkan.cpp
        # ${imgui_include}/backends/imgui_impl_wgpu.cpp
        # ${imgui_include}/backends/imgui_impl_win32.cpp
    )

else()
    message(SEND_ERROR "Imgui in ${imgui_include} has not been found")
endif()

