cmake_minimum_required(VERSION 3.5)

set(PROJECT_NAME "dear_imgui")
PROJECT(${PROJECT_NAME})


set(CMAKE_BUILD_TYPE "Realse")
set(CMAKE_CXX_FLAGS
    "-std=c++11"
)

set(GLFW_PATH  "../glfw-3.3.8/")
set(IMGUI_PATH "./")

include_directories(
    ${IMGUI_PATH}
    ${IMGUI_PATH}/backends
    ${GLFW_PATH}/include
)

set(SOURCE_CODES
    ${IMGUI_PATH}/backends/imgui_impl_glfw.cpp
    ${IMGUI_PATH}/backends/imgui_impl_opengl3.cpp
    ${IMGUI_PATH}/imgui.cpp
    ${IMGUI_PATH}/imgui_demo.cpp
    ${IMGUI_PATH}/imgui_draw.cpp
    ${IMGUI_PATH}/imgui_tables.cpp
    ${IMGUI_PATH}/imgui_widgets.cpp
)

add_library(${PROJECT_NAME} STATIC ${SOURCE_CODES})
