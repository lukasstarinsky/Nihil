cmake_minimum_required(VERSION 3.27)
project(VulkanRenderer LANGUAGES CXX)

file(GLOB_RECURSE HEADER_FILES Source/*.hpp)
file(GLOB_RECURSE SOURCE_FILES Source/*.cpp)

find_package(Vulkan REQUIRED)

add_library(${PROJECT_NAME} SHARED ${HEADER_FILES} ${SOURCE_FILES})

if (WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE VK_USE_PLATFORM_WIN32_KHR)
endif()
target_compile_definitions(${PROJECT_NAME} PRIVATE NIHIL_EXPORT)
target_include_directories(${PROJECT_NAME} PUBLIC Source ../Engine/Source)
target_link_libraries(${PROJECT_NAME} PRIVATE Vulkan::Vulkan)
target_precompile_headers(${PROJECT_NAME} PRIVATE Source/pch.hpp)