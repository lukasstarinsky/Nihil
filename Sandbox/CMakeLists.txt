cmake_minimum_required(VERSION 3.27)
project(NihilSandbox LANGUAGES CXX)

file(GLOB_RECURSE HEADER_FILES src/*.hpp)
file(GLOB_RECURSE SOURCE_FILES src/*.cpp)

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} PRIVATE NihilCore)