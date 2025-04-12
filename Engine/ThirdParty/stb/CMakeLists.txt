cmake_minimum_required(VERSION 3.27)
project(stb LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(${PROJECT_NAME} STATIC src/stb_image.cpp include/stb/stb_image.h)
target_include_directories(${PROJECT_NAME} PUBLIC include)