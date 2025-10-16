cmake_minimum_required(VERSION 3.27)
project(NihilMathUnitTests LANGUAGES CXX)

enable_testing()

find_package(GTest CONFIG REQUIRED)

add_executable(${PROJECT_NAME}
    Mat4.tests.cpp
    Vec3.tests.cpp
    UUID.tests.cpp
    Rect.tests.cpp
)
target_link_libraries(${PROJECT_NAME} PRIVATE GTest::gtest_main NihilCore)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})