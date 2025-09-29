cmake_minimum_required(VERSION 3.27)
project(Nihil LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(Core)
add_subdirectory(Engine)
add_subdirectory(Sandbox)

#add_subdirectory(VulkanRenderer)
add_subdirectory(OpenGLRenderer)