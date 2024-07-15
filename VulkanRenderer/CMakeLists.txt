cmake_minimum_required(VERSION 3.27)
project(VulkanRenderer LANGUAGES CXX)

file(GLOB_RECURSE HEADER_FILES src/*.hpp)
file(GLOB_RECURSE SOURCE_FILES src/*.cpp)

find_package(Vulkan REQUIRED)

add_library(${PROJECT_NAME} SHARED ${HEADER_FILES} ${SOURCE_FILES})
target_compile_definitions(${PROJECT_NAME} PRIVATE NIHIL_EXPORT)
target_include_directories(${PROJECT_NAME} PRIVATE src)
target_link_libraries(${PROJECT_NAME} PRIVATE NihilEngine Vulkan::Vulkan)