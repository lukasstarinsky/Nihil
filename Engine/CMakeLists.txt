cmake_minimum_required(VERSION 3.27)
project(Nihil LANGUAGES CXX)

file(GLOB_RECURSE HEADER_FILES Source/*.hpp)
file(GLOB_RECURSE SOURCE_FILES Source/*.cpp)

if(NOT WIN32)
    file(GLOB_RECURSE WIN32_HEADER_FILES Source/Platform/Win32/*.hpp)
    file(GLOB_RECURSE WIN32_SOURCE_FILES Source/Platform/Win32/*.cpp)
    list(REMOVE_ITEM HEADER_FILES ${WIN32_HEADER_FILES})
    list(REMOVE_ITEM SOURCE_FILES ${WIN32_SOURCE_FILES})
endif()

add_subdirectory(ThirdParty/stb)

find_package(unofficial-shaderc CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(zstd CONFIG REQUIRED)

add_custom_command(
    OUTPUT "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Assets"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/Assets" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Assets"
    COMMENT "Copying assets to output directory"
)
add_custom_target(copy_assets ALL DEPENDS "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Assets")

add_library(${PROJECT_NAME} STATIC ${HEADER_FILES} ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC Source)
target_precompile_headers(${PROJECT_NAME} PRIVATE Source/pch.hpp)
target_link_libraries(${PROJECT_NAME} PUBLIC NihilCore PRIVATE unofficial::shaderc::shaderc stb assimp::assimp zstd::libzstd_shared)
add_dependencies(${PROJECT_NAME} NihilCore OpenGLRenderer copy_assets)