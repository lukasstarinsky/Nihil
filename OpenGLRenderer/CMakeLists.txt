cmake_minimum_required(VERSION 3.27)
project(OpenGLRenderer LANGUAGES CXX)

file(GLOB_RECURSE HEADER_FILES Source/*.hpp)
file(GLOB_RECURSE SOURCE_FILES Source/*.cpp)

if(NOT WIN32)
    list(REMOVE_ITEM SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/Source/OpenGLPlatformWin32.cpp)
endif()

find_package(OpenGL REQUIRED)

add_library(${PROJECT_NAME} SHARED ${HEADER_FILES} ${SOURCE_FILES})

target_compile_definitions(${PROJECT_NAME} PRIVATE NIHIL_EXPORT)
target_include_directories(${PROJECT_NAME} PRIVATE Source ThirdParty)
target_link_libraries(${PROJECT_NAME} PRIVATE NihilCore OpenGL::GL stb)
target_precompile_headers(${PROJECT_NAME} PRIVATE Source/pch.hpp)