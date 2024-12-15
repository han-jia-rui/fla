cmake_minimum_required(VERSION 3.15)

file(GLOB_RECURSE lib_sources ${CMAKE_CURRENT_SOURCE_DIR}/*.cc)

add_library(${PROJECT_LIB_NAME} ${lib_sources})

target_include_directories(${PROJECT_LIB_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)

