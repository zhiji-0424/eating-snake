cmake_minimum_required(VERSION 3.5)

set(PROJECT_NAME "stb_image")
PROJECT(${PROJECT_NAME})


set(CMAKE_BUILD_TYPE "Realse")
set(CMAKE_CXX_FLAGS
    "-std=c++11"
)

set(STB_IMAGE_PATH "./")

include_directories(
    ${STB_IMAGE_PATH}
)

set(SOURCE_CODES
    ${STB_IMAGE_PATH}/stb_image.cpp
)

add_library(${PROJECT_NAME} STATIC ${SOURCE_CODES})
