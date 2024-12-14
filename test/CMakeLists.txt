find_package(Catch2 REQUIRED)
include(Catch)

file(GLOB_RECURSE test_sources ${PROJECT_SOURCE_DIR}/test/*.cc)
file(GLOB_RECURSE fla_sources ${PROJECT_SOURCE_DIR}/fla-project/src/*.cc)

add_executable(fla_test ${test_sources} ${fla_sources})
target_link_libraries(fla_test PRIVATE Catch2::Catch2WithMain)
target_include_directories(fla_test PUBLIC ${PROJECT_SOURCE_DIR}/fla-project/include)

catch_discover_tests(fla_test)
