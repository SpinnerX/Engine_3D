set(glad_include /usr/local/include/glad)

# NOTE
# If glad is not found then go this website to install it and move the folders to /usr/local/include/ as /usr/local/include/glad

# NOTE #2 - Follow these steps if the link below in Glad Installation Link doesn't work
# 1.) Go to this link: https://glad.dav1d.de/
# 2.) Then specify GL API to specific version
# 3.) Specifying compatible means that glad will include even deprecated function calls, as core does not
# 4. Click "Generate" button
# NOTE: Or if you just want to install it without doing that for just easiness then click the link below
# Glad Installation Link: https://glad.dav1d.de/generated/tmptmea9cbvglad/
# We want to check if this directory exists if not then we proceed
if(EXISTS ${glad_include})
    message(STATUS "Glad in /usr/local/include/glad has been found")
    include_directories(
        ${glad_include}/include
        ${glad_include}/include/glad
    )
    set(glad_src ${glad_include}/src/glad.c)
else()
    message(SEND_ERROR "Glad in /usr/local/include/glad was not found")
endif()