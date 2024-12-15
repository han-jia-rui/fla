cmake_minimum_required(VERSION 3.15)

project(fla-tests LANGUAGES CXX)

find_package(Catch2 REQUIRED)
include(Catch)

file(GLOB_RECURSE test_sources ${PROJECT_SOURCE_DIR}/test/*.cc)
file(GLOB_RECURSE fla_sources ${PROJECT_SOURCE_DIR}/../fla-project/src/*.cc)

add_executable(${PROJECT_NAME} ${test_sources} ${fla_sources})
target_link_libraries(${PROJECT_NAME} PRIVATE Catch2::Catch2WithMain)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/fla-project/include)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 14)

enable_testing()
catch_discover_tests(${PROJECT_NAME})
