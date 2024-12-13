cmake_minimum_required(VERSION 3.15.0)

project(FLA
        LANGUAGES CXX)

# ---- Include guards ----

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    message(
    FATAL_ERROR
      "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
  )
endif()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) # disable compiler-specific extensions
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # generate compile_commands.json
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_executable(fla)

target_include_directories(fla
                            PUBLIC ${PROJECT_SOURCE_DIR}/include
                            PRIVATE ${PROJECT_SOURCE_DIR}/src)

file(GLOB_RECURSE fla_sources src/*.cc)
target_sources(fla PRIVATE ${fla_sources})

target_compile_options(fla PRIVATE -Wall -Wextra -Werror -Wpedantic)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)
endif()

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
    add_subdirectory(test)
endif()

