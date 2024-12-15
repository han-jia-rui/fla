cmake_minimum_required(VERSION 3.15)

find_package(Catch2 3 REQUIRED)
include(Catch)

file(GLOB_RECURSE test_sources ${CMAKE_CURRENT_SOURCE_DIR}/*.cc)

add_executable(${PROJECT_TEST_NAME} ${test_sources})

target_link_libraries(${PROJECT_TEST_NAME} PRIVATE ${PROJECT_LIB_NAME} Catch2::Catch2WithMain)

catch_discover_tests(${PROJECT_TEST_NAME})
