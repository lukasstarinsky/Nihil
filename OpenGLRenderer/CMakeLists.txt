cmake_minimum_required(VERSION 3.27)
project(OpenGLRenderer LANGUAGES CXX)

file(GLOB_RECURSE HEADER_FILES Source/*.hpp)
file(GLOB_RECURSE SOURCE_FILES Source/*.cpp)

find_package(OpenGL REQUIRED)

add_library(${PROJECT_NAME} SHARED ${HEADER_FILES} ${SOURCE_FILES})

target_compile_definitions(${PROJECT_NAME} PRIVATE NIHIL_EXPORT)
target_include_directories(${PROJECT_NAME} PRIVATE Source ThirdParty ../Engine/Source)
target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL)
target_precompile_headers(${PROJECT_NAME} PRIVATE Source/pch.hpp)