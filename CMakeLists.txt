# min required
cmake_minimum_required(VERSION 3.21)

project(LearnOpengl)
set(TARGET LearnOpengl)
set(CMAKE_CXX_STANDARD 17)
set(ROOT_PATH ${CMAKE_SOURCE_DIR})

# for load shader texture
add_definitions(-DRESOURCE_DIR="${ROOT_PATH}/resource")

# set different value according platform
set(GLFW_INCLUDE_PATH "")
set(GLFW_LIB "")
set(GLAD_INCLUDE_PATH "")
set(GLAD_SRC "")
set(STB_INCLUDE_PATH "")
set(GLM_INCLUDE_PATH "")

set(OPENGL_INCLUDE_PATH "")
set(OPENGL_LIB_PATH "")
set(OPENGL_LINK_LIBS "")

# set value
message("CMAKE_SYSTEM_NAME is ${CMAKE_SYSTEM_NAME} \n")
if("${CMAKE_SYSTEM_NAME}" MATCHES "Darwin")
    set(GLFW_INCLUDE_PATH ${ROOT_PATH}/thirdparty/mac/glfw/include)
    set(GLAD_INCLUDE_PATH ${ROOT_PATH}/thirdparty/mac/glad/include/)
    set(STB_INCLUDE_PATH ${ROOT_PATH}/thirdparty/mac/stb/include/)
    set(GLM_INCLUDE_PATH ${ROOT_PATH}/thirdparty/mac/glm/include/)

    set(GLFW_LIB_PATH ${ROOT_PATH}/thirdparty/mac/glfw/lib/)
    set(GLFW_LIB ${GLFW_LIB_PATH}/libglfw3.a)

    set(GLAD_SRC ${ROOT_PATH}/thirdparty/mac/glad/src/glad.c)

    set(OPENGL_LINK_LIBS ${OPENGL_LINK_LIBS}
        libz.a
        libbz2.a
        libiconv.a
        lzma
        "-framework AudioToolbox"
        "-framework VideoToolbox"
        "-framework CoreVideo"
        "-framework CoreAudio"
        "-framework CoreMedia"
        "-framework CoreImage"
        "-framework CoreFoundation"
        "-framework AppKit"
        "-framework OpenGL"
	    "-framework IOKit" 
        "-framework Security")
elseif("${CMAKE_SYSTEM_NAME}" MATCHES "Windows")

endif()

set(OPENGL_INCLUDE_PATH 
    ${OPENGL_INCLUDE_PATH}
    ${GLFW_INCLUDE_PATH}
    ${GLAD_INCLUDE_PATH}
    ${GLM_INCLUDE_PATH}
    ${STB_INCLUDE_PATH}
)

set (HDR_PATH  ${ROOT_PATH}/include/)

set(HDR 
    ${ROOT_PATH}/include/public.h
)

set(SRC
    ${ROOT_PATH}/src/main.cpp
)

add_executable(${TARGET} ${HDR} ${SRC} ${GLAD_SRC})
target_include_directories(${TARGET} PUBLIC ${HDR_PATH} ${OPENGL_INCLUDE_PATH})
target_link_libraries(${TARGET} ${GLFW_LIB}  ${OPENGL_LINK_LIBS})

message("------ opengl project end() ------ \n")
