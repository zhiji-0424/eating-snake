cmake_minimum_required(VERSION 3.5)

set(PROJECT_NAME "snake-desktop")
PROJECT(${PROJECT_NAME})
set(EXE_FILE_NAME "snake-desktop")

set(CMAKE_BUILD_TYPE "Realse")
set(CMAKE_CXX_FLAGS
    "-std=c++11 -Wall -Wextra"
)

set(GLFW_PATH  "./glfw-3.3.8/")
set(IMGUI_PATH "./imgui-1.89.4/")
set(SRC_PATH   "./src/")

include_directories(
    ${IMGUI_PATH}
    ${IMGUI_PATH}/backends
    ${GLFW_PATH}/include
)

set(SOURCE_CODES
    ${SRC_PATH}/main.cpp
)

add_executable(${EXE_FILE_NAME} WIN32 ${SOURCE_CODES})

target_link_libraries(${EXE_FILE_NAME}
    ${PROJECT_SOURCE_DIR}/bin/libglfw3.a
    ${PROJECT_SOURCE_DIR}/bin/libdear_imgui.a
    opengl32.lib
)
