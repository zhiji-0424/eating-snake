cmake_minimum_required(VERSION 3.5)

set(PROJECT_NAME "snake-desktop")
PROJECT(${PROJECT_NAME})
set(EXE_FILE_NAME "snake-desktop")

set(CMAKE_BUILD_TYPE "Realse")
set(CMAKE_CXX_FLAGS
    "-std=c++11 -Wall -Wextra"
)

set(GLFW_PATH  "../Snake-core/glfw-3.3.8/")
set(IMGUI_PATH "../Snake-core/imgui-1.89.4/")
set(STB_IMAGE_PATH "../Snake-core/stb_image/")
set(SRC_PATH   "../Snake-core/snake/src/")

include_directories(
    ${IMGUI_PATH}
    ${IMGUI_PATH}/backends
    ${GLFW_PATH}/include
)

set(SOURCE_CODES
    src/desktop-main.cpp
)

add_executable(${EXE_FILE_NAME} WIN32 ${SOURCE_CODES}) #TODO: 使用if判断是否使用 WIN32 标签

target_link_libraries(${EXE_FILE_NAME}
    ${PROJECT_SOURCE_DIR}/prebuilt/bin/libglfw3.a
    ${PROJECT_SOURCE_DIR}/prebuilt/bin/libdear_imgui.a
    ${PROJECT_SOURCE_DIR}/prebuilt/bin/libstb_image.a
    opengl32.lib
)
