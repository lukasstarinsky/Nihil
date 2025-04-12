cmake_minimum_required(VERSION 3.27)
project(NihilCore LANGUAGES CXX)

file(GLOB_RECURSE HEADER_FILES Source/*.hpp)
file(GLOB_RECURSE SOURCE_FILES Source/*.cpp)

add_subdirectory(Tests)

add_subdirectory(ThirdParty/stb)

find_package(unofficial-shaderc CONFIG REQUIRED)

add_library(${PROJECT_NAME} SHARED ${HEADER_FILES} ${SOURCE_FILES})
target_compile_definitions(${PROJECT_NAME} PRIVATE NIHIL_EXPORT)
target_include_directories(${PROJECT_NAME} PUBLIC Source)
target_precompile_headers(${PROJECT_NAME} PRIVATE Source/pch.hpp)
target_link_libraries(${PROJECT_NAME} PRIVATE unofficial::shaderc::shaderc stb)