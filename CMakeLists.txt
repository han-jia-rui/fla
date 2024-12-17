cmake_minimum_required(VERSION 3.15.0)

# ---- Project ----
project(fla
        VERSION 0.1
        LANGUAGES CXX)

# ---- Include guards ----

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    message(
        FATAL_ERROR
        "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
    )
endif()

# ---- Options ----

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) # disable compiler-specific extensions
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # generate compile_commands.json
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

set(PROJECT_TEST_NAME ${PROJECT_NAME}-test)
set(PROJECT_LIB_NAME ${PROJECT_NAME}-lib)

# ---- Library ----

add_subdirectory(fla-project/src)

# ---- App ----

add_subdirectory(fla-project/app)

# ---- Test ----

find_package(Catch2)
if(Catch2_FOUND)
    include(CTest)
    add_subdirectory(fla-project/test)
else()
    message(STATUS "Catch2 not found, not building test")
endif()

# ---- Docs ----

find_package(Doxygen)
if(Doxygen_FOUND)
    add_subdirectory(fla-project/docs)
else()
    message(STATUS "Doxygen not found, not building docs")
endif()
